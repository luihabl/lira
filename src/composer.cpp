#include "composer.h"

#include "assets/content.h"
#include "assets/ldtk.h"
#include "assets/tileset.h"

#include "components/tilemap.h"
#include "components/player.h"
#include "components/actor.h"
#include "components/animated_sprite.h"
#include "components/collider.h"
#include "components/collider_grid.h"
#include "components/timer.h"
#include "components/multitimer.h"

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
    
    auto * tilemap = entity->add_component(TileMap(level_w, level_h, grid_size, grid_size));
    auto * collider = entity->add_component(ColliderGrid(level_w, level_h, grid_size, grid_size)); 

    std::vector<int> cx;
    std::vector<int> cy;
    std::vector<int> t;

    for (const auto & gtiles : level_layer.grid_tiles) {
        cx.push_back((int) (gtiles.px[0] / level_layer.grid_size));
        cy.push_back((int) (gtiles.px[1] / level_layer.grid_size));
        t.push_back((int) gtiles.t);

        collider->set_cell((int) (gtiles.px[0] / level_layer.grid_size), (int) (gtiles.px[1] / level_layer.grid_size), true);
    }

    std::filesystem::path filepath = Content::file_folder<LDTk::File>(name) / *(level_layer.tileset_rel_path.get());
    std::filesystem::path key = filepath.parent_path().stem() / filepath.stem();

    TileSet tileset(16, 16, Content::find<Texture>(key.generic_string().c_str()));

    
    tilemap->set_cells(tileset, cx, cy, t);

    return entity;
}


Entity * Composer::create_player(Scene * scene, std::string name, const TinySDL::IVec2 & position, const int layer) {
    auto* entity = scene->add_entity(position, layer);
    entity->add_component(Player());

    
    auto * animator = entity->add_component(AnimatedSprite(name));
    animator->play("idle");

    auto * collider = entity->add_component(Collider({-7, -1, 14, 17}));

    auto * actor = entity->add_component(Actor());
    actor->collider = collider;

    return entity;
}

Entity* Composer::create_turret(Scene* scene, const TinySDL::IVec2& position, const int layer)
{
    auto* entity = scene->add_entity(position, layer);

    auto* animator = entity->add_component(AnimatedSprite("sprites/turret"));
    animator->play("idle");

    auto* timer = entity->add_component(MultiTimer({
            {0.0f, [](MultiTimer* self) {
                self->get_sibling<AnimatedSprite>()->play("opening");
            }},
            {animator->get("opening")->lenght(), [](MultiTimer* self) {
                self->get_sibling<AnimatedSprite>()->play("preparing");
            }},
            {4.0f * animator->get("preparing")->lenght(), [](MultiTimer* self) {
                self->get_sibling<AnimatedSprite>()->play("shooting");
            }},
            {2.0f * animator->get("preparing")->lenght(), [](MultiTimer* self) {
                self->get_sibling<AnimatedSprite>()->play("closing");
            }},
            {animator->get("closing")->lenght(), [](MultiTimer* self) {
                self->get_sibling<AnimatedSprite>()->play("idle");
            }},
            {2.0f, [](MultiTimer* self) {}}
    }));

    auto* collider = entity->add_component(Collider({ 0, 0, 16, 16 }));


    return entity;
}
