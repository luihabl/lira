#pragma once

#include <cstdint>

namespace Lira
{

    namespace Layer
    {
        namespace Collision
        {
            static constexpr uint32_t solid = 1 << 0;
            static constexpr uint32_t danger = 1 << 1;
            static constexpr uint32_t player = 1 << 2;
            static constexpr uint32_t item = 1 << 3;          
        }

        enum class Draw : int
        {
            base = 0,
            map = -1,
            player = 0,
            items = 0,
            UI = 2,
            overlay = 4,
            UI2 = 6
        };
    }
}