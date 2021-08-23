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

    
    auto * animator = entity->add_component(AnimatedSprite());
    
    auto * walk_animation = animator->add("walk");
    float walk_delay = 90.0f;
    walk_animation->frames.push_back({TexRegion(Content::find<Texture>("sprites/player2"), Rect(0, 0, 32, 32)), {16, 16}, walk_delay});
    walk_animation->frames.push_back({TexRegion(Content::find<Texture>("sprites/player3"), Rect(0, 0, 32, 32)), {16, 16}, walk_delay});
    walk_animation->frames.push_back({TexRegion(Content::find<Texture>("sprites/player4"), Rect(0, 0, 32, 32)), {16, 16}, walk_delay});
    walk_animation->frames.push_back({TexRegion(Content::find<Texture>("sprites/player5"), Rect(0, 0, 32, 32)), {16, 16}, walk_delay});

    auto * idle_animation = animator->add("idle");
    idle_animation->frames.push_back({TexRegion(Content::find<Texture>("sprites/player1"), Rect(0, 0, 32, 32)), {16, 16}, 150.0f});

    auto * jump_animation = animator->add("jump");
    jump_animation->frames.push_back({TexRegion(Content::find<Texture>("sprites/player6"), Rect(0, 0, 32, 32)), {16, 16}, 150.0f});

    animator->play("idle");

    auto * collider = entity->add_component(Collider({-7, -1, 14, 17}));

    auto * actor = entity->add_component(Actor());
    actor->collider = collider;

    /*
        Debug area below vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    */ 

    // auto test_entity = scene->add_entity({20, 20});

    // auto test_collider = test_entity->add_component(Collider({-8, 0, 16, 16}));














    return entity;
}