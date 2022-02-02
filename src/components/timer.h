#pragma once 

#include "../modules/game.h"
#include "../core/ecs.h"
#include <functional>

namespace Lira
{
    class Timer : public Component
    {

    public:

        bool loop = false;
        
        Timer() = default;
        Timer(float _duration, std::function<void(Timer*)> _timeout = nullptr);

        void update() override;
        void set_duration(float _duration);
        void start(float _duration = -1.0f);
        void stop();
        
        bool is_completed() { return completed; }

    private:

        float counter = 0.0f;
        float duration = 0.0f;
        bool completed = false;

        std::function<void(Timer*)> timeout = nullptr;

    };
}