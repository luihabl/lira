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
        VirtualButton::Group buttons;

        void begin() override {

            right = VirtualButton().add(Key::Right).add(Key::D);
            left =  VirtualButton().add(Key::Left).add(Key::A);
            up = VirtualButton().add(Key::Up).add(Key::W);
            down = VirtualButton().add(Key::Down).add(Key::S);

            buttons = VirtualButton::Group({&right, &left, &up, &down});
        }

        void render(BatchRenderer & renderer) override {

            renderer.set_texture(tex);
            renderer.draw_tex({0, 0, (float) tex->w, (float) tex->h}, {(float) entity->position[0], (float) entity->position[1]});
        }


        void update() {
            buttons.update();
            if (right.just_pressed() || right.released())
                entity->position[0] += 5;
        }
    };




}
