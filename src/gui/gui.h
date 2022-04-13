#pragma once 

#include <tinysdl.h>
#include "../lira.h"

namespace Lira
{
    namespace GUI
    {
        void init();
        void end();
        void input(SDL_Event& event);
        void render(LiraGame* game);
        void draw(LiraGame* game);
    }
}