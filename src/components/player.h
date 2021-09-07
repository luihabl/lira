#pragma once

#include <tinysdl.h>
#include "../core/component.h"
#include "../input/virtualbutton.h"
#include "../input/virtualaxis.h"
#include "../modules/game.h"

#include "animated_sprite.h"
#include "actor.h"


using namespace TinySDL;

namespace MicroNinja {

    class Player : public Component {

    public:

        Player() = default;

        VirtualButton right, left, up, down, jump;
        VirtualAxis horizontal_input, vertical_input;
        AnimatedSprite * animator;
        Actor * actor;

        float jump_speed = -150.0f;
        int jump_counter = 0;
        int n_jumps = 2;
        
        bool on_ground = true;


        void begin() override {
            
            jump.add(Key::Z)
                .add(Key::Space)
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

            //Collider* coll = get_sibling<Collider>();
            //coll->on_collide = [this](const Component& c) {
            //    scene()->queue_remove(entity);
            //};
        }


        void update() {

            on_ground = actor->on_ground();
            
            float h_input = horizontal_input.value();
            float v_input = vertical_input.value();

            if (Mathf::sign(h_input) != 0) 
                animator->scale = {Mathf::sign(h_input), 1.0f};

            
            if(h_input != 0) {
                animator->play("walk");
            }
            else {
                animator->play("idle");
            }

            if(!on_ground) {
                animator->play("jump");
            }
            
            
            if(jump.just_pressed() && jump_counter < n_jumps - 1) {
                actor->velocity[1] = jump_speed;
                jump_counter++;
            }
            
            if (on_ground) {
                jump_counter = 0;
            }


            actor->velocity[1] += 600.0f * GameProperties::delta_time();
            actor->velocity[0] = 80.0f * h_input;            

        }
    };




}
