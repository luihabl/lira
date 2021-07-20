#include "input.h"
#include "keys.h"

#include <tinysdl.h>

#include <SDL.h>
#include <cstring>
#include <memory>

using namespace MicroNinja;
using namespace TinySDL;

namespace {
    int n_keys = 512;
    const bool * current_keyboard_state = (bool*) SDL_GetKeyboardState(&n_keys);
    std::unique_ptr<bool[]> previous_keyboard_state(new bool[n_keys]);
}

void Input::update(InputHandler & handler) {
    std::memcpy(previous_keyboard_state.get(), current_keyboard_state, sizeof(uint8_t) * n_keys);

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {

        if (event.type == SDL_WINDOWEVENT) {
            if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                int w, h;

                Window::get_drawable_size(&w, &h);
                handler.on_window_resize(w, h);
            }
        }

        if (event.type == SDL_QUIT) {
            handler.on_quit();
        }      



    }
}

bool Input::pressed(Key k) {
    return current_keyboard_state[(int) k];
}

bool Input::just_pressed(Key k) {
    return current_keyboard_state[(int) k] && !previous_keyboard_state.get()[(int) k];
}

bool Input::released(Key k) {
    return !current_keyboard_state[(int) k] && previous_keyboard_state.get()[(int) k];
}