#pragma once

#include "../core/ecs.h"
#include "../modules/game.h"
#include <auk.h>

#include <functional>

namespace Lira
{
    class AnimatedDrawing : public Component
    {
    public:
        std::function<void(AnimatedDrawing* self, auk::BatchRenderer& renderer, float t)> draw = nullptr;
        float t = 0.0f;
        bool started = false;

        
        void update() override
        {
            if(started)
                t += GameProperties::delta_time();
        }

        void render(auk::BatchRenderer& renderer) override
        {
            // This guarantees that the render will be called at t = 0
            started = true;
            if(draw)
                draw(this, renderer, t);
        }
        
    };
}
