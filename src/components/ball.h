#pragma once

#include <tinysdl.h>
#include "../core/component.h"
#include "../core/entity.h"
#include "../input/input.h"

using namespace TinySDL;

namespace MicroNinja {

    class Ball : public Component {

    public:
        int w = 10;
        int h = 10;

        Texture * tex = nullptr;

        Ball() = default;

        void render(BatchRenderer & renderer) override {

            
            renderer.draw_circle_fill({(float) entity->position[0], (float) entity->position[1]}, 5, Color::red);
        }


        void update() {
            entity->position = Input::get_mouse_window_pos();  
        }
    };




}
