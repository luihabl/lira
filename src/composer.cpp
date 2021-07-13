#include "composer.h"

#include "assets/content.h"
#include "assets/ldtk.h"
#include "assets/tileset.h"

#include "components/tilemap.h"

#include <filesystem>

using namespace MicroNinja;
using namespace TinySDL;

Entity * Composer::create_map(Scene * scene, std::string name, const IVec2 & position, const int layer) {

    auto* entity = scene->add_entity(position, layer);

    auto* map = Content::find<LDTk::File>(name);

    auto & l = *(map->levels[0].layer_instances);
    auto l2 = l[0];

    std::vector<int> cx;
    std::vector<int> cy;
    std::vector<int> t;

    for (const auto & gtiles : l2.grid_tiles) {
        cx.push_back((int) gtiles.px[0] / 16);
        cy.push_back((int) gtiles.px[1] / 16);
        t.push_back((int) gtiles.t);
    }

    std::filesystem::path filepath = Content::file_folder<LDTk::File>(name) / *(l2.tileset_rel_path.get());
    std::filesystem::path key = filepath.parent_path().stem() / filepath.stem();

    TileSet tileset(16, 16, Content::find<Texture>(key.generic_string().c_str()));

    auto * component = entity->add_component(TileMap(16, 16, 16, 16));
    component->set_cells(tileset, cx, cy, t);

    return entity;
}