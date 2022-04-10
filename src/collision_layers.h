#pragma once

namespace Lira
{
    namespace CollisionLayer
    {
        static constexpr uint32_t solid = 1 << 0;
        static constexpr uint32_t danger = 1 << 1;
        static constexpr uint32_t player = 1 << 2;
        static constexpr uint32_t item = 1 << 3;
    };

}