#pragma once 

#include "modules/game.h"

using namespace MicroNinja;
using namespace TinySDL;

namespace MicroNinja  {

    class MicroNinjaGame : public Game {

        using Game::Game;

        void begin() override;
        void render() override;

    public:
        RenderTarget target;

    };

}

