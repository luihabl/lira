#pragma once

#include "keys.h"
#include <functional>

namespace MicroNinja
{
    struct InputHandler {

        std::function<void()> on_quit;

        std::function<void(int, int)> on_window_resize;

    };


    namespace Input {

        bool just_pressed(Key k);
        bool pressed(Key k);
        bool released(Key k);

        void update(InputHandler & handler);
    }
} 
