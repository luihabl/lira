#pragma once

namespace Lira
{
    namespace Sound
    {
        void init();
        void play(const char* name);
        void stop(const char* name);
        void update();
        void stop_all();
        void terminate();
    }
}

