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
#include "components/animated_drawing.h"
#include "components/persistance.h"

#include "layers.h"

#include <filesystem>
#include <algorithm>
#include <unordered_map>

#include <iostream>

using namespace Lira;
using namespace TinySDL;

Entity * Composer::create_level(Scene * scene, std::string name, size_t level_n, const IVec2 & position, const Layer::Draw& render_layer) {

    auto* map = Content::find<Map>(name);
    
    if(!map) 
        return nullptr;

    auto& room = map->rooms[level_n];

    auto* entity = scene->add_entity(position + room.bbox.pos(), (int) render_layer);

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
                        collider_solid->layer = Layer::Collision::solid;
                    }
                        
                    collider_solid->set_cell(tile.x, tile.y, true);
                }

                if (tile.flag == Map::TileFlag::DANGER)
                {
                    if (!collider_danger)
                    {
                        collider_danger = entity->add_component(ColliderGrid(layer.columns, layer.rows, layer.dx, layer.dy));
                        collider_danger->layer = Layer::Collision::danger;
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
            {
                auto* en = create_player(scene, { object.pos[0] + room.bbox.x, object.pos[1] + room.bbox.y}, Layer::Draw::player);
                en->is_active = false;
                create_start_sequence(scene, {0, 0}, Layer::Draw::overlay);
            }
        }

        if (object.name == "Heart")
        {
            create_heart(scene, { object.pos[0] + room.bbox.x, object.pos[1] + room.bbox.y }, Layer::Draw::items);
        }

        if (object.name == "Crystal")
        {
            create_crystal(scene, { object.pos[0] + room.bbox.x, object.pos[1] + room.bbox.y }, Layer::Draw::items);
        }
    }

    return entity;
}


Entity * Composer::create_player(Scene * scene, const TinySDL::IVec2 & position, const Layer::Draw& layer) {
    auto* entity = scene->add_entity(position, (int) layer);

    auto * animator = entity->add_component(AnimatedSprite("sprites/player_lira"));
    animator->play("idle");

    auto * collider = entity->add_component(Collider({-6, -16, 12, 16}));
    collider->layer = Layer::Collision::player;

    auto * actor = entity->add_component(Actor());
    actor->collider = collider;
    actor->solid_mask = Layer::Collision::solid;

    entity->add_component(Player());

    entity->add_component(Persistence());

    return entity;
}

Entity* Composer::create_turret(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer)
{
    auto* entity = scene->add_entity(position, (int)layer);

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

Entity* Composer::create_bullet(Scene* scene, const Vec2& direction, const TinySDL::IVec2& position, const Layer::Draw& layer)
{
    auto* entity = scene->add_entity(position, (int)layer);
    
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

Entity* Composer::create_heart(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer)
{
    auto* entity = scene->add_entity(position, (int)layer);
    auto* spr = entity->add_component(AnimatedSprite());
    spr->static_sprite(
        {
            TexRegion(Content::find<Texture>("sprites/heart"), Rect(0, 0, 8, 8)),
            {0, 0}
        });

    auto* collider = entity->add_component(Collider({ 0, 2, 7, 6 }));
    collider->layer = Layer::Collision::item;

    auto* hittable = entity->add_component(Hittable());
    hittable->hit_by = Layer::Collision::player;
    hittable->collider = collider;
    hittable->on_hit = [=](Hittable* self)
    {
        auto* player = self->scene()->get_first<Player>();
        if(player)
            player->recover(1);

        create_collect_effect(scene, position + IVec2({4, 4}), Color::red, layer);
        self->entity->destroy();
    };

    return entity;
}


Entity* Composer::create_crystal(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer)
{
    auto* entity = scene->add_entity(position, (int)layer);
    auto* spr = entity->add_component(AnimatedSprite());
    spr->static_sprite(
        {
            TexRegion(Content::find<Texture>("sprites/crystal"), Rect(0, 0, 8, 8)),
            {0, 0}
        });

    auto* collider = entity->add_component(Collider({ 0, 0, 8, 8 }));
    collider->layer = Layer::Collision::item;

    auto* hittable = entity->add_component(Hittable());
    hittable->hit_by = Layer::Collision::player;
    hittable->collider = collider;
    hittable->on_hit = [=](Hittable* self)
    {
        auto* player = self->scene()->get_first<Player>();
        if(player)
            player->recharge_dash();

        create_collect_effect(scene, position + IVec2({4, 4}), Color::green, layer);
        self->entity->destroy();
    };

    return entity;
}


Entity* Composer::create_collect_effect(Scene* scene, const TinySDL::IVec2& position, const Color& color, const Layer::Draw& layer)
{
    auto* entity = scene->add_entity(position, (int)layer);

    float r0 = 4.0f;
    float r_rate = 25.f;
    float tf = r0 / r_rate;
    float v = 8.0f / tf;
    int n_drw = 8;
    float dth = 2.0f * Mathf::pi / (float)n_drw;

    auto* animation = entity->add_component(AnimatedDrawing());
    animation->draw = [=](AnimatedDrawing* self, BatchRenderer& renderer, float t)
    {
        for (int i = 0; i < n_drw; i++)
        {
            Vec2 pos = self->entity->position.cast_to<float>() + Vec2({cosf(dth * (float)i) * v, sinf(dth * (float)i) * v}) * t;
            renderer.draw_circle_fill(pos, r0 - t * r_rate, color, 15);
        }
    };

    entity->add_component(Timer(tf, [](Timer* self) {
            self->entity->destroy();
    }));

    return entity;
}

Entity* Composer::create_hp_bar(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer)
{
    auto* entity = scene->add_entity(position, (int)layer);

    auto tex = TexRegion(Content::find<Texture>("sprites/heart"), Rect(0, 0, 8, 8));
    auto tex_empty = TexRegion(Content::find<Texture>("sprites/heart_empty"), Rect(0, 0, 8, 8));


    // TODO: Find better solution for HP bar.
    // Maybe create a HPBar component for further animation
    // or create another way to flexibly animate the HP bar.
    // Not great to have an AnimatedDrawing calling a lambda function 
    // for redering the HP and making its logic at the same time.
    auto *anim = entity->add_component(AnimatedDrawing());
    anim->draw = [=](AnimatedDrawing* self, BatchRenderer& renderer, float t)
    {
        auto* player = self->scene()->get_first<Player>();
        if(player)
        {
            int hp = player->get_hp();
            int hp_max = player->get_max_hp();

            for(int i = 0; i < hp; i++)
            {
                renderer.draw_tex(tex, position.cast_to<float>() + Vec2({(float)i * 8.0f, 0.0f}));
            }

            for(int i = hp; i < hp_max; i++)
            {
                renderer.draw_tex(tex_empty, position.cast_to<float>() + Vec2({(float)i * 8.0f, 0.0f}));
            }
        }
    };

    entity->add_component(Persistence());

    return entity;
}


Entity* Composer::create_end_sequence(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer)
{
    auto* entity = scene->add_entity(position, (int)layer);

    float tf = 0.5f;
    float dx = 50.0f;

    auto *anim = entity->add_component(AnimatedDrawing());
    anim->draw = [tf, dx](AnimatedDrawing* self, BatchRenderer& renderer, float t)
    {
        float x = (320.0f + dx) * t / tf - dx;

        renderer.draw_rect_fill({0, 0, x, 180}, Color::black);
        renderer.draw_triangle_fill({x, 0.0f}, {x, 180.0f}, {x + dx, 180.0f}, Color::black);
    };
    
    entity->add_component(Timer(tf, [](Timer* self)
    {
        self->is_active = false;
        auto* player = self->scene()->get_first<Player>();
        if(player)
            player->entity->destroy();

        self->entity->destroy();
    }));

    return entity;
}

Entity* Composer::create_start_sequence(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer)
{
    auto* entity = scene->add_entity(position, (int)layer);

    float tf = 0.5f;
    float dx = 50.0f;

    auto *anim = entity->add_component(AnimatedDrawing());
    anim->draw = [tf, dx](AnimatedDrawing* self, BatchRenderer& renderer, float t)
    {
        // renderer.draw_rect_fill({320.0f * (t / tf), 0, 320.0f, 180}, Color::black);

        float x = (320.0f + dx) * t / tf;

        renderer.draw_rect_fill({x, 0, 320.0f, 180.0f}, Color::black);
        renderer.draw_triangle_fill({x - dx, 0.0f}, {x, 0.0f}, {x, 180.0f}, Color::black);
    };

    entity->add_component(Timer(tf, [](Timer* self)
    {
        self->is_active = false;
        auto* player = self->scene()->get_first<Player>();
        if(player)
            player->entity->is_active = true;

        self->entity->destroy();
    }));

    return entity;
}
