#pragma once

#include <tinysdl.h>
#include "../core/component.h"
#include "../input/virtualbutton.h"
#include "../input/virtualaxis.h"
#include "sprite.h"

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
        VirtualAxis horizontal_input;
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


            animator = entity->add_component(AnimatedSprite());
            
            auto * walk_animation = animator->add("walk");
            walk_animation->frames.push_back({TexRegion(Content::find<Texture>("sprites/player"), Rect(0, 0, 32, 32)), {16, 0}, 150.0f});
            walk_animation->frames.push_back({TexRegion(Content::find<Texture>("sprites/player"), Rect(5, 0, 32, 32)), {16, 0}, 150.0f});
            
            animator->play("walk");
        
        }

        void render(BatchRenderer & renderer) override {

            // renderer.set_texture(tex);
            // renderer.draw_tex({0, 0, (float) tex->w, (float) tex->h}, {(float) entity->position[0], (float) entity->position[1]});
        }


        void update() {
            
            float h_input = horizontal_input.value();

            if (Mathf::sign(h_input) != 0) {
                //modify AnimatedSprite scale

                animator->scale = {Mathf::sign(h_input), 1.0f};
                Log::debug("HInput: %f", Mathf::sign(h_input));
            }

            entity->position[0] += 5 * (int) horizontal_input.value();
        
        }
    };




}
