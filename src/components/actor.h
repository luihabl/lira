#pragma once

#include "../core/component.h"
#include "collider.h"

#include <tinysdl.h>

namespace MicroNinja {

    class Actor : public Component {

    public:
        Collider* collider = nullptr;

        TinySDL::Vec2 velocity = {0, 0};
        TinySDL::Vec2 move_remainder = {0, 0};

        void move_x(float amount);
        void move_y(float amount);
        
        void move_exact_x(int amount);
        void move_exact_y(int amount);
    
        void update() override;
    
    };
}