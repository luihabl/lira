#pragma once

#include <tinysdl.h>
#include "../core/component.h"
#include "../input/virtualbutton.h"
#include "../input/virtualaxis.h"
#include "../util/state_machine.h"
#include "../modules/game.h"

#include "animated_sprite.h"
#include "actor.h"
#include "timer.h"

//#include <array>


using namespace TinySDL;

namespace Lira {

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
        float max_slide_speed = 50.0f;

        float dash_length = 15.0f;
        float dash_max_speed = 220.0f;
        float dash_accel = 150.0f;
        float dash_recharge_delay = 0.2f;
        
        float gravity = 600.0f;
        float current_gravity = gravity;
        
        float air_accel = 300.0f;
        float jump_speed = -150.0f;
        float wall_jump_speed = 170.0f;
        int jump_counter = 0;
        int n_jumps = 2;
        int wall_jump_margin = 3;

        int facing = 1;

        bool on_ground = true;
        bool was_on_ground = true;
        bool on_wall = false;
        bool was_on_wall = false;

        bool sliding = false;
        float slide_gravity_multiplier = 0.18f;

        bool is_dashing = false;
        bool is_recharging_dash = false;
        float dash_counter = 0.0f;

        bool invincible = false;
        int invincible_counter = 0;

        struct Trail
        {
            IVec2 pos;
            float radius;
            bool remove = false;

            void render(BatchRenderer& renderer)
            {
                renderer.draw_circle_fill(pos.cast_to<float>() + Vec2({0.0f, -6.0f}), radius, Color::white);
                radius -= 0.75f;
                if (radius <= 0)
                    remove = true;
            }
        };
        
        std::vector<Trail> trails;

        StateMachine<AnimatedSprite> animation_states;
        static constexpr size_t anim_jump = 0;
        static constexpr size_t anim_walk = 1;
        static constexpr size_t anim_slide = 2;
        static constexpr size_t anim_fall = 3;
        static constexpr size_t anim_idle = 4;
        static constexpr size_t anim_dash = 5;
               
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

            animator->connect("intojump", "jump loop");
            animator->connect("wall jump", "jump loop");
            animator->connect("intofall", "fall loop");
            animator->connect("landing", "idle");
            animator->connect("into dash", "dash loop");

            actor = get_sibling<Actor>();
            actor->auto_update = false;

            animation_states.add(anim_jump, [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s) 
                {
                    if (s->get_last() == anim_slide)
                        a->play("wall jump");
                    else
                        a->play("intojump");
                });
            animation_states.add(anim_walk, [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s) { a->play("walk");});
            animation_states.add(anim_slide, [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s) {a->play("cling loop"); });
            animation_states.add(anim_fall, [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s) {a->play("intofall"); }, nullptr,
                [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s)
                {
                    if (s->get_state() == anim_walk || s->get_state() == anim_idle)
                    {
                        a->scale = { 1.35f, 0.65f }; // make this depend on the impact velocity
                    }
                        
                });
            animation_states.add(anim_idle, [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s)
                {
                    if (s->get_last() == anim_fall || s->get_last() == anim_jump)
                    {
                        a->play("landing");
                    } 
                    else
                        a->play("idle");
                });
            animation_states.add(anim_dash, 
            [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s) {
                a->is_visible = false;
                a->play("into dash"); 
            }, 
            nullptr,
            [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s){
                a->is_visible = true;
            });
        }

        void update() {
            set_movement();
            set_animations();
        }

        void set_movement()
        {
            Vec2& velocity = actor->velocity;
            
            was_on_ground = on_ground;
            on_ground = actor->on_ground();

            was_on_wall = on_wall;
            on_wall = actor->on_wall(1) || actor->on_wall(-1);

            current_gravity = gravity;

            if (Mathf::sign(horizontal_input.value()) != 0)
                facing = (int) Mathf::sign(horizontal_input.value());

            // Jump

            if (on_ground) 
            {
                jump_counter = 0;
            }

            if (jump.just_pressed() && jump_counter < n_jumps) 
            {
                bool on_wall_margin_r = actor->on_wall(wall_jump_margin);
                bool on_wall_margin_l = actor->on_wall(-wall_jump_margin);
                if ((on_wall_margin_r || on_wall_margin_l) && !on_ground)
                {                    
                    if (on_wall_margin_r)
                    {
                        velocity[0] = -wall_jump_speed;
                        velocity[1] = 1.1f * jump_speed;
                    }
                    else if (on_wall_margin_l)
                    {
                        velocity[0] = wall_jump_speed;
                        velocity[1] = 1.1f * jump_speed;
                    }   
                }
                else
                {
                    velocity[1] = jump_speed;
                }

                animator->scale = {0.7f, 1.3f};

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
                entity->add_component(Timer(dash_recharge_delay , [this](Timer* self) {
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

            // Wall slide

            sliding = false;
            if (on_wall)
            {
                if (!on_ground && velocity[1] >= 0 && horizontal_input.value() != 0)
                {
                    sliding = true;
                    current_gravity = slide_gravity_multiplier * gravity;
                    //velocity[1] = std::min(velocity[1], max_slide_speed);
                    // Change friction!
                    
                    jump_counter = jump_counter > 1 ? 1 : jump_counter;
                    // jump_counter = 1;
                }
            }

            // Vertical movement
            
            velocity[1] += current_gravity * GameProperties::delta_time();

            if (abs(velocity[1] > 4 * floor_max_speed) && !sliding)
            {
                velocity[1] = Mathf::approach(velocity[1], vertical_max_speed * Mathf::sign(velocity[1]), 1000 * GameProperties::delta_time());
            }
            else if (sliding)
            {
                velocity[1] = Mathf::approach(velocity[1], max_slide_speed, 1000 * GameProperties::delta_time());
            }

            bool hit = actor->collider->check_first(IVec2::zeros, 2);
            if (hit)
            {
                if(!invincible)
                {
                    invincible_counter = 0;
                    invincible = true;

                    velocity = Vec2({- ((float) facing) * 150.0f, -120.0f});

                    entity->add_component(Timer(1.0f , [this](Timer* self) {
                        invincible = false;
                        animator->is_visible = true;
                        self->destroy();
                    }));
                }
            }

            actor->move_x();
            actor->move_y();
        }

        void set_animations()
        {
            if (horizontal_input.value() != 0 && on_ground && actor->velocity[1] >= 0) {
                animation_states.set(anim_walk, animator);
            }
            else if (on_ground && actor->velocity[1] >= 0) {
                animation_states.set(anim_idle, animator);
            }

            if (!on_ground && !is_dashing && !sliding) {
                if (actor->velocity[1] < 0) {
                    animation_states.set(anim_jump, animator);
                }
                else if (actor->velocity[1] > 0) {
                    animation_states.set(anim_fall, animator);
                }
                    
            }

            if (is_dashing)
            {
                animation_states.set(anim_dash, animator);
 
                trails.push_back({entity->position, 5.0f, false});

                for (int i = (int)trails.size() - 1; i >= 0; i--)
                {
                    if (trails[i].remove)
                    {
                        trails.erase(trails.begin() + i);
                    }
                }
            }
            else
            {

                trails.clear();
            }

            if (sliding)
            {
                animator->flip_x = true;
                animation_states.set(anim_slide, animator);
            }
            else
            {
                animator->flip_x = false;
            }

            if (invincible)
            {
                if (invincible_counter % 4 == 0)
                    animator->is_visible = !animator->is_visible;

                invincible_counter++;
            }


            animator->scale[0] = std::abs(animator->scale[0]) * (float)facing;
            animator->scale = Mathf::approach(animator->scale, {(float)facing, 1.0f}, GameProperties::delta_time() * 4.0f);
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
