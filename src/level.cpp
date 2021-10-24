
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

    //Find first instance of player in LDtk map.
    //Not sure this is the best way to start since
    //You cannot switch rooms during debug
    int player_room_id = find_in_map("Player");
    player_room_id = player_room_id < 0 ? 0 : player_room_id;


    current_room = rooms[player_room_id];
    camera = { current_room.bbox.x, current_room.bbox.y };

    load_room(current_room.id);
 
    Scene::begin();
}

void Level::set_map_info(const std::string& _map_name)
{
    map_name = _map_name;
    const auto map = Content::find<LDTk::File>(map_name);
    
    size_t id = 0;
    for(const auto& level : map->levels)
    {
        rooms.push_back(Room({ id++, { (int)level.world_x, (int)level.world_y, (int)level.px_wid, (int)level.px_hei } }));
    }
}

// This finds the player "entity" in the loaded map
// However maybe we want to change this in the future to be able to select rooms
int Level::find_in_map(const std::string& name)
{
    const auto map = Content::find<LDTk::File>(map_name);
    for (int id = 0; id < map->levels.size(); id++)
    {
        auto layers = map->levels[id].layer_instances.get();
        if (!layers) continue;

        for (const auto& layer : *layers)
        {
            if (layer.identifier == "Entities")
            {
                for (const auto& en : layer.entity_instances)
                {
                    if (en.identifier == name)
                        return id;
                }
            }
        }
    }
    return -1;
}

void Level::load_room(size_t id)
{
    Composer::create_level(this, map_name, id, {0, 0}, -1);
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
        if(!current_room.bbox.contains(player->entity->position))
        {           
            const auto& pos = player->entity->position;
            for(size_t i = 0; i < rooms.size(); i++)
            {
                if(rooms[i].bbox.contains(pos))
                {
                    unload_room();

                    current_room = rooms[i];

                    load_room(current_room.id);
                    camera = { current_room.bbox.x, current_room.bbox.y };
                }
            }
        }

        if (current_room.bbox.w > room_default_width)
        {
            camera[0] = Mathf::clamp(player->entity->position[0] - room_default_width / 2, current_room.bbox.x, current_room.bbox.x + current_room.bbox.w - room_default_width);
        }

        if (current_room.bbox.h > room_default_height)
        {
            camera[1] = Mathf::clamp(player->entity->position[1] - room_default_height / 2, current_room.bbox.y, current_room.bbox.y + current_room.bbox.h - room_default_height);
        }
    }
}

void Level::render(TinySDL::BatchRenderer& renderer)
{
    Graphics::clear(Color::black);

    renderer.push_transform(LinAlg2D::gen_translation((float) -camera[0], (float) -camera[1]));

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


    // vvvv camera-independent items vvvv
    
    /*
        Move this to component later.
        Maybe this type of component that can be rendered always on screen, 
        independent of the camera. Canvas?
    */
    if (render_minimap)
    {
        renderer.draw_rect_fill(Rect( 0.0f, 0.0f, (float)room_default_width, (float)room_default_height ), Color(0, 0, 50, 150));

        renderer.push_transform(LinAlg2D::gen_transform({ (float)room_default_width * 0.25f, (float)room_default_height * 0.5f }, { 0.1f, 0.1f }, {0.0f, 0.0f}, 0.0f));

        for (size_t i = 0; i < rooms.size(); i++)
        {
            renderer.draw_rect_line(rooms[i].bbox.cast<float>(), {42, 53, 99 }, 10);
        }

        renderer.draw_rect_line(current_room.bbox.cast<float>(), { 48, 145, 54 }, 10);

        const auto* player = get_first<Player>();
        const auto& pos = player->entity->position.cast_to<float>();
        renderer.draw_rect_fill(Rect( pos[0], pos[1], 10.0f, 20.0f ), Color( 161, 80, 53 ));
        
        renderer.pop_transform();

    }

}
