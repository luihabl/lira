
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

    auto* e2 = add_entity({ 40, 40 }, -1);
    e2->add_component(SimpleSprite(Content::find<Texture>("sprites/dude_transparent")));

    auto* e3 = add_entity({ 50, 120 });
    e3->add_component(SimpleSprite(Content::find<Texture>("sprites/dude_transparent")));

    auto* e4 = add_entity({ 50, 130 },  2);
    e4->add_component(SimpleSprite(Content::find<Texture>("sprites/dude_transparent")));

    // queue_remove(c);

    Scene::begin();
}


void Level0::update() {
    // Add logic here
    // Input should be captured in the main game class
    

    Scene::update();
}