#include "input.h"
#include "keys.h"

#include <tinysdl.h>

#include <SDL.h>
#include <cstring>
#include <memory>

using namespace MicroNinja;


namespace {
    int n_keys = 512;
    const uint8_t * current_keyboard_state = SDL_GetKeyboardState(&n_keys);
    std::unique_ptr<uint8_t[]> previous_keyboard_state(new uint8_t[n_keys]);
}

void Input::update() {
    std::memcpy(previous_keyboard_state.get(), current_keyboard_state, sizeof(uint8_t) * n_keys);
    SDL_PumpEvents();
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