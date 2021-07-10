
#include <tinysdl.h>

#include "level0.h" 
#include "tinysdl.h"
#include "assets/content.h"
#include "modules/game.h"
#include "components/simple_sprite.h"

using namespace MicroNinja;
using namespace TinySDL;


void Level0::begin() {
    // Add entities and components here...

    auto* e1 = add_entity({ 0, 0 }, -10);
    auto* c = e1->add_component(SimpleSprite(Content::find_tex("tilesets/Game Boy GBC - Pokemon Gold Silver - Cinnabar Island")));

    auto* e2 = add_entity({ 50, 40 }, -1);
    e2->add_component(SimpleSprite(Content::find_tex("sprites/dude_transparent")));

    auto* e3 = add_entity({ 50, 120 });
    e3->add_component(SimpleSprite(Content::find_tex("sprites/dude_transparent")));

    auto* e4 = add_entity({ 50, 130 },  2);
    e4->add_component(SimpleSprite(Content::find_tex("sprites/dude_transparent")));

    destroy_entity(e2);

    Scene::begin();
}


void Level0::update() {
    // Add logic here
    // Input should be captured in the main game class
    

    Scene::update();
}