#pragma once

#include "keys.h"

#include <tinysdl.h>

#include <functional>

namespace MicroNinja
{
    struct InputHandler {

        std::function<void()> on_quit;

        std::function<void(int, int)> on_window_resize;

    };

    class VirtualInput;

    namespace Input {

        bool just_pressed(Key k);
        bool pressed(Key k);
        bool released(Key k);

        TinySDL::IVec2 get_mouse_window_pos();
        TinySDL::IVec2 get_mouse_global_pos();
        
        void register_button(VirtualInput * button);
        void unregister_button(VirtualInput * button);

        void update(InputHandler & handler);
    }
} 
