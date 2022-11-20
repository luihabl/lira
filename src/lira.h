#pragma once 

#include "modules/game.h"

using namespace Lira;
using namespace auk;

namespace Lira  {

    class LiraGame : public Game {

        using Game::Game;

        void begin() override;
        void update() override;
        void render() override;
        void end() override;

    public:
        RenderTarget target;

    };

}

