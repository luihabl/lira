#pragma once

namespace Lira
{
    namespace Sound
    {
        void init();
        void play(const char* name);
        void stop();
        void stop_all();
        void terminate();
    }
}

