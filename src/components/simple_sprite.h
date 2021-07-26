#pragma once

#include <tinysdl.h>
#include "../core/component.h"
#include "../input/virtualbutton.h"
#include "../input/virtualaxis.h"

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

        }

        void render(BatchRenderer & renderer) override {

            renderer.set_texture(tex);
            renderer.draw_tex({0, 0, (float) tex->w, (float) tex->h}, {(float) entity->position[0], (float) entity->position[1]});
        }


        void update() {

            entity->position[0] += 5 * (int) horizontal_input.value();
        
        }
    };




}
