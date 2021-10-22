
#include <tinysdl.h>

#include "level.h" 
#include "composer.h"
#include "modules/game.h"
#include "input/input.h"

#include "assets/ldtk.h"
#include "assets/content.h"

#include "components/collider.h"
#include "components/collider_grid.h"
#include "components/player.h"

using namespace MicroNinja;
using namespace TinySDL;


void Level::begin() {

    set_map_info("tilemaps/map1");


    current_room_id = 1;
    
    load_room(current_room_id);
    current_room_bbox = bbox_rooms[current_room_id];
    camera_offset = {current_room_bbox.x, current_room_bbox.y};

    Scene::begin();
}

void Level::set_map_info(const std::string& _map_name)
{
    current_map_name = _map_name;

    const auto map = Content::find<LDTk::File>(current_map_name);
    
    for(const auto& level : map->levels)
    {
        bbox_rooms.push_back({(int)level.world_x, (int)level.world_y, (int)level.px_wid, (int)level.px_hei});
    }
}

void Level::load_room(size_t id)
{
    auto* room = Composer::create_level(this, current_map_name, id, {0, 0}, -1);
}

void Level::unload_room()
{
    for(Entity* entity : get_entities())
    {
        if(!entity->get_component<Player>())
        {
            entity->destroy();
        }
    }
}

void Level::update() {
    
    Scene::update();

    if (Input::just_pressed(Key::F1))
        render_colliders = !render_colliders;

    if (Input::just_pressed(Key::Q)) // Move to component later
        render_minimap = !render_minimap;


    const auto* player = get_first<Player>();
    if(player)
    {
        if(!current_room_bbox.contains(player->entity->position))
        {           
            const auto& pos = player->entity->position;
            for(size_t i = 0; i < bbox_rooms.size(); i++)
            {
                if(bbox_rooms[i].contains(pos))
                {
                    unload_room();

                    current_room_id = i;

                    load_room(current_room_id);
                    current_room_bbox = bbox_rooms[current_room_id];
                    camera_offset = { current_room_bbox.x, current_room_bbox.y };
                }
            }
        }

        if (current_room_bbox.w > room_default_width)
        {

            camera_offset[0] = Mathf::clamp(player->entity->position[0] - room_default_width / 2, current_room_bbox.x, current_room_bbox.x + current_room_bbox.w - room_default_width);
        }

        if (current_room_bbox.h > room_default_height)
        {
            camera_offset[1] = Mathf::clamp(player->entity->position[1] - room_default_height / 2, current_room_bbox.y, current_room_bbox.y + current_room_bbox.h - room_default_height);
        }
    }
}

void Level::render(TinySDL::BatchRenderer& renderer)
{
    Graphics::clear(Color::black);

    renderer.push_transform(LinAlg2D::gen_translation((float) -camera_offset[0], (float) -camera_offset[1]));

    Scene::render(renderer);
    
    if (render_colliders)
    {
        auto& colliders = get_components<Collider>();
        auto& grid_colliders = get_components<ColliderGrid>();

        for (auto* c : colliders)
            c->render(renderer);

        for (auto* c : grid_colliders)
            c->render(renderer);
    }

    /*
        Move this to component later.
        Maybe this type of component that can be rendered always on screen, 
        independent of the camera. Canvas?
    */
    if (render_minimap)
    {
        renderer.push_transform(LinAlg2D::gen_translation((float)camera_offset[0], (float)camera_offset[1]));

        renderer.draw_rect_fill(Rect({ 0.0f, 0.0f, (float)room_default_width, (float)room_default_height }), {0, 0, 50, 150});
        
        float scale = 0.1f;

        renderer.push_transform(LinAlg2D::gen_translation((float)room_default_width * 0.25f, (float)room_default_height * 0.5f));

        for (size_t i = 0; i < bbox_rooms.size(); i++)
        {
            
            auto bbox = bbox_rooms[i].cast_to<float>();
            bbox = Rect{ bbox.x * scale, bbox.y * scale, bbox.w * scale, bbox.h * scale };

            renderer.draw_rect_line(bbox, {42, 53, 99, 255}, 1);            
        }

        renderer.draw_rect_line(Rect({ (float)current_room_bbox.x * scale, (float)current_room_bbox.y * scale, (float)current_room_bbox.w * scale, (float)current_room_bbox.h * scale }), { 48, 145, 54 }, 1);

        const auto* player = get_first<Player>();
        const auto& pos = player->entity->position.cast_to<float>();
        renderer.draw_rect_fill(Rect({ pos[0] * scale, pos[1] * scale, 1.0f, 2.0f }), { 161, 80, 53 });
        
        renderer.pop_transform();
        renderer.pop_transform();
    }




    renderer.pop_transform();
}
