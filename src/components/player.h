#pragma once

#include <tinysdl.h>
#include "../core/component.h"
#include "../input/virtualbutton.h"
#include "../input/virtualaxis.h"
#include "../modules/game.h"

#include "animated_sprite.h"
#include "actor.h"
#include "timer.h"


using namespace TinySDL;

namespace MicroNinja {

    class Player : public Component {

    public:

        Player() = default;

        VirtualButton right, left, up, down, jump, dash;
        VirtualAxis horizontal_input, vertical_input;
        AnimatedSprite * animator;
        Actor * actor;

        
        float floor_max_speed = 85.0f;
        float vertical_max_speed = 340.0f;
        float floor_accel = 700.0f;
        float floor_friction = 650.0f;
        float max_slide_speed = 70.0f;
        
        float dash_length = 15.0f;
        float dash_max_speed = 220.0f;
        float dash_accel = 150.0f;
        
        float gravity = 600.0f;
        float current_gravity = gravity;
        
        float air_accel = 300.0f;
        float jump_speed = -150.0f;
        int jump_counter = 0;
        int n_jumps = 2;
        
        bool on_ground = true;

        bool is_dashing = false;
        bool is_recharging_dash = false;
        float dash_counter = 0.0f;


        struct Trail
        {
            IVec2 pos;
            float radius;
            bool remove = false;

            void render(BatchRenderer& renderer)
            {
                renderer.draw_circle_fill(pos.cast_to<float>() + Vec2({0.0f, 6.0f}), radius, Color::white);
                radius -= 0.75f;
                if (radius <= 0)
                    remove = true;
            }
        };
        
        std::vector<Trail> trails;
        
        void begin() override {
            
            jump.add(Key::Z)
                .register_input();

            dash.add(Key::X)
                .register_input();
           
            up.add(Key::Up)
                .add(Key::W)
                .register_input();

            down.add(Key::Down)
                .add(Key::S)
                .set_repeat(500, 150)
                .register_input();

            horizontal_input.add(Key::Left, Key::Right)
                            .add(Key::A, Key::D)
                            .register_input();

            vertical_input.add(Key::Up, Key::Down)
                            .add(Key::W, Key::S)
                            .register_input();

            animator = get_sibling<AnimatedSprite>();
            actor = get_sibling<Actor>();

        }


        void update() {
            set_movement();
            set_animations();
        }

        void set_movement()
        {
            Vec2& velocity = actor->velocity;

            on_ground = actor->on_ground();

            current_gravity = gravity;

            if (on_ground) 
            {
                jump_counter = 0;
            }

            if (jump.just_pressed() && jump_counter < n_jumps) 
            {
                velocity[1] = jump_speed;
                jump_counter++;
            }

            // Decrease gravity when holding the jump button
            if (jump.pressed()) 
            {
                current_gravity *= 0.75f;
            }
            else
            {
                current_gravity = gravity;
            }

            // Recharge dash after delay
            if (on_ground && !is_dashing && dash_counter >= dash_length && !is_recharging_dash) 
            {
                is_recharging_dash = true;
                entity->add_component(Timer(1.0f , [this](Timer* self) {
                    is_recharging_dash = false;
                    dash_counter = 0.0f;
                    self->destroy();
                }));
            }
            
            if (dash.pressed() && dash_counter < dash_length)
            {
                is_dashing = true;
                dash_counter += 1.0f;
                current_gravity = 0.0f;

                Vec2 dash_direction = { horizontal_input.value(), vertical_input.value() };
                dash_direction = dash_direction.normalized();

                if (dash_direction.length() == 0.0f)
                {
                    dash_direction = velocity.normalized();
                }

                actor->velocity += dash_accel * dash_direction;

                if (velocity.length() > dash_max_speed)
                    velocity = velocity.normalized() * dash_max_speed;
            }
            else
            {
                is_dashing = false;
            }

            // Horizontal movement

            if (!is_dashing)
            {
                if (on_ground)
                    velocity[0] += horizontal_input.value() * floor_accel * GameProperties::delta_time();
                else
                    velocity[0] += horizontal_input.value() * air_accel * GameProperties::delta_time();


                if (abs(velocity[0]) > floor_max_speed)
                    velocity[0] = Mathf::approach(velocity[0],  floor_max_speed * Mathf::sign(velocity[0]), 1000.0f * GameProperties::delta_time());


                if (horizontal_input.value() == 0.0f && on_ground)
                    velocity[0] = Mathf::approach(velocity[0], 0.0f, floor_friction * GameProperties::delta_time());

            }

            // Vertical movement
            
            velocity[1] += current_gravity * GameProperties::delta_time();

            if (abs(velocity[1] > 4 * floor_max_speed))
            {
                velocity[1] = Mathf::approach(velocity[1], vertical_max_speed * Mathf::sign(velocity[1]), 1000 * GameProperties::delta_time());
            }
        }

        void set_animations()
        {
            if (Mathf::sign(horizontal_input.value()) != 0)
                animator->scale = { Mathf::sign(horizontal_input.value()), 1.0f };

            if (horizontal_input.value() != 0) {
                animator->play("run");
            }
            else {
                animator->play("idle");
            }

            if (!on_ground) {
                if (actor->velocity[1] <= 0)
                    animator->play("jump_up");
                else
                    animator->play("jump_down");
            }

            if (is_dashing)
            {
                animator->is_visible = false;
                trails.push_back({entity->position, 5.0f, false});

                for (int i = trails.size() - 1; i >= 0; i--)
                {
                    if (trails[i].remove)
                    {
                        trails.erase(trails.begin() + i);
                    }
                }
            }
            else
            {
                animator->is_visible = true;
                trails.clear();
            }
        }
    

        void render(BatchRenderer& renderer)
        {
            if (is_dashing)
                for (auto& trail : trails)
                {
                    trail.render(renderer);
                }
        }
    };




}
