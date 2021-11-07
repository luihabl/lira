#include "composer.h"

#include "assets/content.h"
// #include "assets/ldtk.h"
#include "assets/map.h"
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
#include <algorithm>
#include <unordered_map>

#include <iostream>

using namespace MicroNinja;
using namespace TinySDL;

Entity * Composer::create_level(Scene * scene, std::string name, size_t level_n, const IVec2 & position, const int render_layer) {

    auto* map = Content::find<Map>(name);
    
    if(!map) 
        return nullptr;

    auto& room = map->rooms[level_n];

    auto* entity = scene->add_entity(position + room.bbox.pos(), render_layer);

    for (const auto& layer : room.layers)
    {
        if(!layer.tileset)
            continue;


        auto* tilemap = entity->add_component(TileMap(layer.columns, layer.rows, layer.dx, layer.dy));
        bool has_flags = tilemap->set_cells(layer);  

        // Optimize flag assignment
        if(has_flags)
        {
            ColliderGrid* collider = nullptr;
            for(const auto& tile : layer.tiles)
            {
                if(tile.flag == Map::TileFlag::SOLID)
                {
                    if(!collider)
                        collider = entity->add_component(ColliderGrid(layer.columns, layer.rows, layer.dx, layer.dy));
                    
                    collider->set_cell(tile.x, tile.y, true);
                }
            }
        }
    }

    for(const auto& object : room.objects)
    {
        if(object.name == "Player")
        {

            if(!scene->get_first<Player>())
                create_player(scene, { object.pos[0] + room.bbox.x, object.pos[1] + room.bbox.y}, 1);

        }
    }



    // auto* map = Content::find<LDTk::File>(name);
    // auto& level = map->levels[level_n];
    // auto& level_layers = *(level.layer_instances.get());

    // IVec2 level_pos = IVec2({(int)level.world_x, (int)level.world_y});

    // auto* entity = scene->add_entity(position + level_pos, layer);


    // struct TilsetInfo
    // {
    //     LDTk::TilesetDefinition def;
    //     std::unordered_map<std::string, std::vector<int>> enums;
    // };


    // std::unordered_map<int, TilsetInfo> tilesets;
    // for (auto& ts :  map->defs.tilesets)
    // {
    //     TilsetInfo info;
    //     info.def = ts;

    //     for(const auto& tag : ts.enum_tags)
    //         info.enums[tag.at("enumValueId")] = tag.at("tileIds").get<std::vector<int>>(); 

    //     tilesets[(int)ts.uid] = info;
    // }
        

    // for (int i = (int)level_layers.size() - 1; i >= 0; i--)
    // {
    //     auto& level_layer = level_layers[i];

    //     if (level_layer.type == "Tiles" || level_layer.type == "IntGrid")
    //     {
    //         int uid = (int) *level_layer.tileset_def_uid.get();
            
    //         std::filesystem::path filepath = Content::file_folder<LDTk::File>(name) / *(level_layer.tileset_rel_path.get());
    //         std::filesystem::path key = filepath.parent_path().stem() / filepath.stem();
            
    //         TileSet tileset((int) tilesets[uid].def.tile_grid_size, (int) tilesets[uid].def.tile_grid_size, Content::find<Texture>(key.generic_string().c_str()));

    //         int grid_size = (int)level_layer.grid_size;
    //         int level_w = (int)(level.px_wid % grid_size == 0 ? level.px_wid / grid_size : level.px_wid / grid_size + 1);
    //         int level_h = (int)(level.px_hei % grid_size == 0 ? level.px_hei / grid_size : level.px_hei / grid_size + 1);

    //         auto* tilemap = entity->add_component(TileMap(level_w, level_h, grid_size, grid_size));

    //         ColliderGrid* collider = nullptr;
    //         std::vector<int>* solid_tiles = nullptr;

    //         if(tilesets[uid].enums.count("Solid"))
    //         {
    //             if (tilesets[uid].enums["Solid"].size() > 0)
    //                 collider = entity->add_component(ColliderGrid(level_w, level_h, grid_size, grid_size));
            
    //             solid_tiles = &(tilesets[uid].enums["Solid"]);
    //         }

    //         std::vector<int> cx, cy, t;  
    //         std::vector<int> f;

    //         std::vector<LDTk::TileInstance> tiles;

    //         if(level_layer.type == "Tiles")
    //             tiles = level_layer.grid_tiles;
    //         if(level_layer.type == "IntGrid")
    //             tiles = level_layer.auto_layer_tiles;

    //         for (const auto& gtiles : tiles) {
    //             cx.push_back((int)(gtiles.px[0] / level_layer.grid_size));
    //             cy.push_back((int)(gtiles.px[1] / level_layer.grid_size));
    //             t.push_back((int)gtiles.t);
    //             f.push_back((int)gtiles.f);             
                   
    //             if(solid_tiles)
    //                 if (std::find(solid_tiles->begin(), solid_tiles->end(), (int)gtiles.t) != solid_tiles->end())
    //                     collider->set_cell((int)(gtiles.px[0] / level_layer.grid_size), (int)(gtiles.px[1] / level_layer.grid_size), true);
    //         }
            
    //         tilemap->set_cells(tileset, cx, cy, t, f);
    //     }



    //     if (level_layer.type == "Entities")
    //     {
    //         for (auto& layer_entity : level_layer.entity_instances)
    //         {
    //             if (layer_entity.identifier == "Turret")
    //             {
    //                 create_turret(scene, { (int) layer_entity.px[0] + level_pos[0], (int) layer_entity.px[1] + level_pos[1]});
    //             }

    //             if (layer_entity.identifier == "Player")
    //             {
    //                 if(!scene->get_first<Player>())
    //                     create_player(scene, { (int) layer_entity.px[0] + level_pos[0], (int) layer_entity.px[1] + level_pos[1]}, 1);
    //             }
                
    //         }
    //     }
    // }


    return entity;
}


Entity * Composer::create_player(Scene * scene, const TinySDL::IVec2 & position, const int layer) {
    auto* entity = scene->add_entity(position, layer);
    entity->add_component(Player());

    
    auto * animator = entity->add_component(AnimatedSprite("sprites/player"));
    animator->play("idle");

    auto * collider = entity->add_component(Collider({-4, 0, 8, 16}));

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
                create_bullet(self->scene(), { -1.0f, 0.0f }, self->entity->position + IVec2({-12, -1}));
            }},
            {1.0f * animator->get("preparing")->lenght(), [](MultiTimer* self) {
                self->get_sibling<AnimatedSprite>()->play("closing");
            }},
            {animator->get("closing")->lenght(), [](MultiTimer* self) {
                self->get_sibling<AnimatedSprite>()->play("idle");
            }},
            {2.0f, []([[maybe_unused]] MultiTimer* self) {}}
    }));

    timer->loop = true;

    entity->add_component(Collider({ 0, 0, 16, 16 }));

    return entity;
}

Entity* Composer::create_bullet(Scene* scene, const Vec2& direction, const TinySDL::IVec2& position, const int layer)
{
    auto* entity = scene->add_entity(position, layer);
    
    auto* animator = entity->add_component(AnimatedSprite("sprites/turret-bullet"));
    animator->play("drift");
    
    auto* actor = entity->add_component(Actor());
    actor->velocity = direction * 100.0f;

    auto* collider = entity->add_component(Collider({ 0, 0, 12, 7 }));
    actor->collider = collider;

    actor->on_collide_x = [](Actor* self)
    {
        self->stop_x();
        auto* animator = self->get_sibling<AnimatedSprite>();
        self->get_sibling<Collider>()->destroy();
        
        animator->play("exploding");

        self->entity->add_component(Timer(animator->get("exploding")->lenght(), [](Timer* self) {
            self->entity->destroy();
        }));

    };

    return entity;
}
