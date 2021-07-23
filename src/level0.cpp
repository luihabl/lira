
#include <tinysdl.h>

#include "level0.h" 
#include "tinysdl.h"
#include "composer.h"
#include "assets/content.h"
#include "modules/game.h"
#include "components/simple_sprite.h"
#include "components/tilemap.h"
#include "assets/ldtk.h"
#include "assets/tileset.h"

#include <iostream>

using namespace MicroNinja;
using namespace TinySDL;


void Level0::begin() {


    auto* e1 = Composer::create_map(this, "tilemaps/map0", {0, 0});

    auto* e2 = Composer::create_player(this, "sprites/player", {0, 0});

    auto* e3 = Composer::create_ball(this, {0, 0});


    Scene::begin();
}


void Level0::update() {
    // Add logic here
    // Input should be captured in the main game class
    

    Scene::update();
}