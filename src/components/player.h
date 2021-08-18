#pragma once

#include <tinysdl.h>
#include "../core/component.h"
#include "../input/virtualbutton.h"
#include "../input/virtualaxis.h"
#include "animated_sprite.h"
#include "actor.h"


using namespace TinySDL;

namespace MicroNinja {

    class Player : public Component {

    public:
        int w = 10;
        int h = 10;

        Player() = default;

        VirtualButton right, left, up, down;
        VirtualAxis horizontal_input, vertical_input;
        AnimatedSprite * animator;
        Actor * actor;

        void begin() override {

           
            up.add(Key::Up)
                .add(Key::W)
                .set_repeat(500, 150)
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
            
            float h_input = horizontal_input.value();
            float v_input = vertical_input.value();

            if (Mathf::sign(h_input) != 0) 
                animator->scale = {Mathf::sign(h_input), 1.0f};

            
            if(h_input != 0 || v_input != 0) {
                animator->play("walk");
            }
            else {
                animator->play("idle");
            }


            actor->velocity[0] = 70.0f * h_input;            
            actor->velocity[1] = 70.0f * v_input;

        }
    };




}
