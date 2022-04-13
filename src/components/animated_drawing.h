#pragma once

#include "../core/ecs.h"
#include "../modules/game.h"
#include <tinysdl.h>

#include <functional>

namespace Lira
{
    class AnimatedDrawing : public Component
    {
    public:
        std::function<void(AnimatedDrawing* self, TinySDL::BatchRenderer& renderer, float t)> draw = nullptr;
        float t = 0.0f;

        void render(TinySDL::BatchRenderer& renderer) override
        {
            if(draw)
                draw(this, renderer, t);

            t += GameProperties::delta_time();
        }


    };
}
