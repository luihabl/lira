#pragma once

#include "../core/component.h"
#include "collider.h"

#include <tinysdl.h>

#include <functional>

namespace MicroNinja {

    class Actor : public Component {

    public:
        Collider* collider = nullptr;

        TinySDL::Vec2 velocity = {0, 0};
        TinySDL::Vec2 move_remainder = {0, 0};

        bool auto_update = true;

        void move_x(float amount);
        void move_y(float amount);
        void move_x();
        void move_y();
        
        void move_exact_x(int amount);
        void move_exact_y(int amount);

        void stop_x();
        void stop_y();

        std::function<void(Actor*)> on_collide_x = nullptr;
        std::function<void(Actor*)> on_collide_y = nullptr;

        bool on_ground(int offset = 1);
        bool on_wall(int dir = 1);
    
        void update() override;
    
    };
}