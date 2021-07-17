#pragma once

#include "keys.h"

namespace MicroNinja
{
    namespace Input {

        bool just_pressed(Key k);
        bool pressed(Key k);
        bool released(Key k);

        void update();
    }
} 
