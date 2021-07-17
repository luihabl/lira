#pragma once

#include <tinysdl.h>
#include "../core/component.h"
#include "../input/input.h"

using namespace TinySDL;

namespace MicroNinja {

    class SimpleSprite : public Component {

    public:
        int w = 10;
        int h = 10;

        Texture * tex = nullptr;

        SimpleSprite() = default;
        SimpleSprite(Texture * tex): tex(tex) {}

        void render(BatchRenderer & renderer) override {

            renderer.set_texture(tex);
            renderer.draw_tex({0, 0, (float) tex->w, (float) tex->h}, {(float) entity->position[0], (float) entity->position[1]});
        }


        void update() {
            
            if (Input::just_pressed(Key::Right))
                entity->position[0] += 5;
        }
    };




}
