#include "input.h"
#include "keys.h"

#include "../modules/game.h"

#include <tinysdl.h>

#include <SDL.h>
#include <cstring>
#include <memory>

using namespace MicroNinja;
using namespace TinySDL;

namespace {
    int n_keys = 512;
    bool * current_keyboard_state = (bool*) SDL_GetKeyboardState(&n_keys);
    std::unique_ptr<bool[]> previous_keyboard_state(new bool[n_keys]);

    IVec2 mouse_global_position = IVec2::zeros;
    IVec2 mouse_window_position = IVec2::zeros;

    IVec2 window_size = IVec2::zeros;
    IVec2 window_position = IVec2::zeros;
}

void Input::update(InputHandler & handler) {
    std::memcpy(previous_keyboard_state.get(), current_keyboard_state, sizeof(uint8_t) * n_keys);

    SDL_GetWindowPosition(Window::get_window(), &window_position[0], &window_position[1]); 
    SDL_GetGlobalMouseState(&mouse_global_position[0], &mouse_global_position[1]);
    mouse_window_position = mouse_global_position - window_position;

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

IVec2 Input::get_mouse_window_pos() {
    return mouse_window_position;
}

IVec2 Input::get_mouse_global_pos() {
    return mouse_global_position;
}