#include "composer.h"

#include "assets/content.h"
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
#include "components/hittable.h"

#include "collision_layers.h"

#include <filesystem>
#include <algorithm>
#include <unordered_map>

#include <iostream>

using namespace Lira;
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
            ColliderGrid* collider_solid = nullptr;
            ColliderGrid* collider_danger = nullptr;

            for(const auto& tile : layer.tiles)
            {
                if(tile.flag == Map::TileFlag::SOLID)
                {
                    if (!collider_solid)
                    {
                        collider_solid = entity->add_component(ColliderGrid(layer.columns, layer.rows, layer.dx, layer.dy));
                        collider_solid->layer = CollisionLayer::solid;
                    }
                        
                    collider_solid->set_cell(tile.x, tile.y, true);
                }

                if (tile.flag == Map::TileFlag::DANGER)
                {
                    if (!collider_danger)
                    {
                        collider_danger = entity->add_component(ColliderGrid(layer.columns, layer.rows, layer.dx, layer.dy));
                        collider_danger->layer = CollisionLayer::danger;
                    }

                    collider_danger->set_cell(tile.x, tile.y, true);
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

        if (object.name == "Heart")
        {
            create_heart(scene, { object.pos[0] + room.bbox.x, object.pos[1] + room.bbox.y }, 1);
        }
    }

    return entity;
}


Entity * Composer::create_player(Scene * scene, const TinySDL::IVec2 & position, const int layer) {
    auto* entity = scene->add_entity(position, layer);

    auto * animator = entity->add_component(AnimatedSprite("sprites/player_lira"));
    animator->play("idle");

    auto * collider = entity->add_component(Collider({-6, -16, 12, 16}));
    collider->layer = CollisionLayer::player;

    auto * actor = entity->add_component(Actor());
    actor->collider = collider;
    actor->solid_mask = CollisionLayer::solid;

    entity->add_component(Player());

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

Entity* Composer::create_heart(Scene* scene, const TinySDL::IVec2& position, const int layer)
{
    auto* entity = scene->add_entity(position, layer);
    auto* spr = entity->add_component(AnimatedSprite());
    spr->static_sprite(
        {
            TexRegion(Content::find<Texture>("sprites/heart"), Rect(0, 0, 8, 8)),
            {0, 0}
        });

    auto* collider = entity->add_component(Collider({ 0, 0, 8, 8 }));
    collider->layer = CollisionLayer::item;

    auto* hittable = entity->add_component(Hittable());
    hittable->hit_by = CollisionLayer::player;
    hittable->collider = collider;
    hittable->on_hit = [](Hittable* self)
    {
        self->entity->destroy();
    };


    return entity;
}
