#pragma once

#include <tinysdl.h>
#include "../core/component.h"
#include "../input/virtualbutton.h"
#include "../input/virtualaxis.h"
#include "animated_sprite.h"


using namespace TinySDL;

namespace MicroNinja {

    class SimpleSprite : public Component {

    public:
        int w = 10;
        int h = 10;

        Texture * tex = nullptr;

        SimpleSprite() = default;
        SimpleSprite(Texture * tex): tex(tex) {}

        VirtualButton right, left, up, down;
        VirtualAxis horizontal_input, vertical_input;
        AnimatedSprite * animator;

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
        
        }

        // void render(BatchRenderer & renderer) override {

            // renderer.set_texture(tex);
            // renderer.draw_tex({0, 0, (float) tex->w, (float) tex->h}, {(float) entity->position[0], (float) entity->position[1]});
        // }


        void update() {
            
            float h_input = horizontal_input.value();

            if (Mathf::sign(h_input) != 0) {
                //modify AnimatedSprite scale

                animator->scale = {Mathf::sign(h_input), 1.0f};
                // Log::debug("HInput: %f", Mathf::sign(h_input));
            }

            float v_input = vertical_input.value();

            // if (Mathf::sign(v_input) != 0) {
                //modify AnimatedSprite scale

                // animator->scale = {Mathf::sign(v_input), 1.0f};
                // animator->flip_y = Mathf::sign(v_input) > 0;
                // Log::debug("HInput: %f", Mathf::sign(h_input));
            // }


            if(h_input != 0 || v_input != 0) {
                animator->play("walk");
            }
            else {
                animator->play("idle");
            }


            entity->position[0] += 2 * (int) h_input;
            entity->position[1] += 2 * (int) v_input;
        
        }
    };




}
