
#include <tinysdl.h>

#include "level.h" 
#include "composer.h"
#include "modules/game.h"
#include "input/input.h"

#include "assets/map.h"
#include "assets/content.h"

#include "components/collider.h"
#include "components/collider_grid.h"
#include "components/player.h"
#include "components/persistance.h"

#include "sound/sound.h"

using namespace Lira;
using namespace TinySDL;


void Level::begin() {

    set_map("tilemaps/LIRA_CG_C1");

    //Find first instance of player in LDtk map.
    //Not sure this is the best way to start since
    //You cannot switch rooms during debug
    int player_room_id = find_in_map("Player");
    player_room_id = player_room_id < 0 ? 0 : player_room_id;

    move_to_room(player_room_id); 
    
    Scene::begin();

    Sound::play("PlayMusic");
    Game::pause_for(0.5f);

    Composer::create_hp_bar(this, {5, 3}, Layer::Draw::UI);
    minimap = Composer::create_minimap(this, {0, 0}, Layer::Draw::UI2);
    minimap->is_visible = false;
    minimap->is_active = false;

    camera_layers = {Layer::Draw::map, Layer::Draw::player, Layer::Draw::items};
}

void Level::end()
{
    Scene::end();
    Sound::stop_all();
}

void Level::set_map(const std::string& _map_name)
{
    map_name = _map_name;
    current_map = Content::find<Map>(map_name);
    
    size_t id = 0;
    for(const auto& room : current_map->rooms)
    {
        rooms.push_back(Room({ id++, room.bbox }));
    }
}

// This finds the player "entity" in the loaded map
// However maybe we want to change this in the future to be able to select rooms
int Level::find_in_map(const std::string& name)
{
    for (int id = 0; id < current_map->rooms.size(); id++)
    {
        for(const auto& o : current_map->rooms[id].objects)
        {
            if(o.name == name)
            {
                return id;
            }
        }
    }
    return -1;
}

void Level::load_room(size_t id)
{
    Composer::create_level(this, map_name, id, {0, 0}, Layer::Draw::map);
}

void Level::unload_room()
{
    for(Entity* entity : last_room_entities)
    {
        if(!entity->get_component<Persistence>())
        {
            entity->destroy();
        }
    }
}

void Level::move_to_room(size_t id)
{
    unload_room();

    current_room = rooms[id];

    load_room(current_room.id);
    camera = { current_room.bbox.x, current_room.bbox.y };
}

IVec2 Level::room_camera_target(const IVec2& player_pos, const Room& room)
{   
    IVec2 target = {room.bbox.x, room.bbox.y}; 

    if (room.bbox.w > room_default_width)
    {
        target[0] = Mathf::clamp(player_pos[0] - room_default_width / 2, room.bbox.x, room.bbox.x + room.bbox.w - room_default_width);
    }
    if (room.bbox.h > room_default_height)
    {
        target[1] = Mathf::clamp(player_pos[1] - room_default_height / 2, room.bbox.y, room.bbox.y + room.bbox.h - room_default_height);
    }

    return target;
}



void Level::update() {
    
    Scene::update();

    // Debug
    if (Input::just_pressed(Key::F1))
        render_colliders = !render_colliders;
    if (Input::just_pressed(Key::Q)) // Move to component later
    {            
        minimap->is_active = !minimap->is_active;
        minimap->is_visible = !minimap->is_visible;
    }
    if (Input::just_pressed(Key::F2))
    {
        const auto* player = get_first<Player>();
        
        if(player)
            player->entity->destroy();
    }
    if (Input::just_pressed(Key::F3))
    {
        Composer::create_player(this, { 125, 125 });
    }

    // Player management
    const auto* player = get_first<Player>();
    if(player)
    {

        // Change room check
        if(!current_room.bbox.contains(player->entity->position) && !room_transition)
        {          
            room_transition = true;
            transition_easing = 0.0f;
            player->entity->is_active = false;

            const auto& pos = player->entity->position;
            for(size_t i = 0; i < rooms.size(); i++)
            {
                if(rooms[i].bbox.contains(pos))
                {
                    next_room = rooms[i];
                    // move_to_room(i);
                }
            }
            
            last_room_entities = get_entities();
            load_room(next_room.id);
        }
        else if(room_transition)
        {
            Vec2 camera_current_target = room_camera_target(player->entity->position, current_room).cast_to<float>();
            Vec2 camera_next_target = room_camera_target(player->entity->position, next_room).cast_to<float>();

            transition_easing = Mathf::approach(transition_easing, 1.0f, GameProperties::delta_time() / 0.6f);
            
            Vec2 camera_f = (camera_current_target + (camera_next_target - camera_current_target) * Mathf::Easing::quartic_ease_in_out(transition_easing));
            camera = {(int) roundf(camera_f[0]), (int) roundf(camera_f[1])};

            if (transition_easing >= 1.0f)
            {
                unload_room();
                last_room_entities.clear();
                player->entity->is_active = true;
                current_room = next_room;
                room_transition = false;
            }
        }
        else
        {
            camera = room_camera_target(player->entity->position, current_room);
        }

        // Restart check
        if (player->get_hp() <= 0 && player->entity->is_active)
        {
            player->entity->is_active = false;
            Composer::create_end_sequence(this, {0, 0}, Layer::Draw::overlay);
        }
    }
    else
    {
        int player_room_id = find_in_map("Player");
        player_room_id = player_room_id < 0 ? 0 : player_room_id;
        
        last_room_entities = get_entities();
        move_to_room(player_room_id);
    }

    for (const auto& layer : camera_layers)
        layer_transform(layer, LinAlg2D::gen_translation((float)-camera[0], (float)-camera[1]));
}


void Level::layer_transform(Layer::Draw layer, const TinySDL::Mat3x2& transform)
{
	Scene::layer_transform((int)layer, transform);
}

void Level::render(TinySDL::BatchRenderer& renderer)
{
    Graphics::clear(Color::black);

    Scene::render(renderer);

    // This is only for debug ===============================
    renderer.push_transform(LinAlg2D::gen_translation((float) -camera[0], (float) -camera[1]));
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
    // =====================================================
}