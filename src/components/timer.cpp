#include "timer.h"

using namespace MicroNinja;

Timer::Timer(float _duration, std::function<void(Timer*)> _timeout)
{
    duration = _duration;
    counter = _duration;
    timeout = _timeout;
}

void Timer::update()
{
    if(counter > 0.0f)
    {
        counter -= GameProperties::delta_time();
        completed = false;
        if(counter <= 0.0f) {
            completed = true;
            if(timeout) timeout(this);
            if(loop) counter = duration;
        }
    }
}

void Timer::set_duration(float _duration)
{
    duration = _duration;
}

void Timer::start(float _duration)
{
    if(_duration > 0)
        duration = _duration;
    
    counter = duration;
}

void Timer::stop()
{
    counter = 0.0f;
}