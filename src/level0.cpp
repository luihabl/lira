
#include <tinysdl.h>

#include "level0.h" 
#include "tinysdl.h"
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
    // Add entities and components here...

    auto* map = Content::find<LDTk::File>("tilemaps/map0");

    auto & l = *(map->levels[0].layer_instances);
    auto l2 = l[0];

    std::vector<int> cx;
    std::vector<int> cy;
    std::vector<int> t;

    for (const auto & gtiles : l2.grid_tiles) {
        cx.push_back(gtiles.px[0] / 16);
        cy.push_back(gtiles.px[1] / 16);
        t.push_back(gtiles.t);
    }

    TileSet tileset(16, 16, Content::find<Texture>("tilesets/Game Boy GBC - Pokemon Gold Silver - Cinnabar Island"));


    auto* e1 = add_entity({ 0, 0 }, -10);
    // auto* c = e1->add_component(SimpleSprite(Content::find<Texture>("tilesets/Game Boy GBC - Pokemon Gold Silver - Cinnabar Island")));
    auto * c = e1->add_component(TileMap(16, 16, 16, 16));
    c->set_cells(tileset, cx, cy, t);

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