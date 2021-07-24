#pragma once

#include <tinysdl.h>
#include "../core/component.h"
#include "../input/virtualbutton.h"

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

        void begin() override {

            right.add(Key::Right)
                 .add(Key::D)
                 .register_input();

            left.add(Key::Left)
                .add(Key::A)
                .register_input();
           
            up.add(Key::Up)
                .add(Key::W)
                .register_input();

            down.add(Key::Down)
                .add(Key::S)
                .register_input();

        }

        void render(BatchRenderer & renderer) override {

            renderer.set_texture(tex);
            renderer.draw_tex({0, 0, (float) tex->w, (float) tex->h}, {(float) entity->position[0], (float) entity->position[1]});
        }


        void update() {

            if (right.just_pressed() || right.released())
                entity->position[0] += 5;
        }
    };




}
