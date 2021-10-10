
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

    renderer.pop_transform();
}
