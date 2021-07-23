#include "composer.h"

#include "assets/content.h"
#include "assets/ldtk.h"
#include "assets/tileset.h"

#include "components/tilemap.h"
#include "components/simple_sprite.h"
#include "components/ball.h"

#include <filesystem>

using namespace MicroNinja;
using namespace TinySDL;

Entity * Composer::create_map(Scene * scene, std::string name, const IVec2 & position, const int layer) {

    auto* entity = scene->add_entity(position, layer);

    auto* map = Content::find<LDTk::File>(name);

    auto & level = map->levels[0];
    auto & level_layer = (*(level.layer_instances.get()))[0];

    int grid_size = (int) level_layer.grid_size;
    int level_w = (int) (level.px_wid % grid_size == 0 ? level.px_wid / grid_size : level.px_wid / grid_size + 1);
    int level_h = (int) (level.px_hei % grid_size == 0 ? level.px_hei / grid_size : level.px_hei / grid_size + 1);

    std::vector<int> cx;
    std::vector<int> cy;
    std::vector<int> t;

    for (const auto & gtiles : level_layer.grid_tiles) {
        cx.push_back((int) (gtiles.px[0] / level_layer.grid_size));
        cy.push_back((int) (gtiles.px[1] / level_layer.grid_size));
        t.push_back((int) gtiles.t);
    }

    std::filesystem::path filepath = Content::file_folder<LDTk::File>(name) / *(level_layer.tileset_rel_path.get());
    std::filesystem::path key = filepath.parent_path().stem() / filepath.stem();

    TileSet tileset(16, 16, Content::find<Texture>(key.generic_string().c_str()));

    auto * component = entity->add_component(TileMap(level_w, level_h, grid_size, grid_size));
    component->set_cells(tileset, cx, cy, t);

    return entity;
}


Entity * Composer::create_player(Scene * scene, std::string name, const TinySDL::IVec2 & position, const int layer) {
    auto* entity = scene->add_entity(position, layer);
    entity->add_component(SimpleSprite(Content::find<Texture>(name)));

    return entity;
}

Entity * Composer::create_ball(Scene * scene, const TinySDL::IVec2 & position) {
    auto* entity = scene->add_entity(position, 0);
    entity->add_component(Ball());

    return entity;
}