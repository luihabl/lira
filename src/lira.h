#pragma once 

#include "modules/game.h"

using namespace Lira;
using namespace TinySDL;

namespace Lira  {

    class LiraGame : public Game {

        using Game::Game;

        void begin() override;
        void render() override;

    public:
        RenderTarget target;

    };

}

