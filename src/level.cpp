
#include <auk.h>

#include "level.h" 
#include "composer.h"
#include "modules/game.h"
#include "input/input.h"

#include "assets/map.h"
#include "assets/content.h"

#include "components/collider.h"
#include "components/collider_grid.h"
#include "components/player.h"
#include "components/persistence.h"

#include "sound/sound.h"

using namespace Lira;
using namespace auk;


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

    Composer::create_hp_bar(this, {{5, 3}, ""}, Layer::Draw::UI);
    minimap = Composer::create_minimap(this, {{0, 0}, ""}, Layer::Draw::UI2);
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
    camera = { (float) current_room.bbox.x, (float) current_room.bbox.y };
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
        Composer::create_player(this, {{ 125, 125 }, ""});
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
            
            camera = Mathf::ease(Mathf::Easing::quartic_in_out, camera_current_target, camera_next_target, transition_easing);            
            // camera = {(int) roundf(camera_f[0]), (int) roundf(camera_f[1])};

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
            // camera =  Mathf::approach(camera.cast_to<float>(), room_camera_target(player->entity->position, current_room).cast_to<float>(), GameProperties::delta_time() * 100.0f).cast_to<int>();
            // Vec2 camera_f = Mathf::ease(Mathf::Easing::linear_interpolation, camera.cast_to<float>(), room_camera_target(player->entity->position, current_room).cast_to<float>(), GameProperties::delta_time() * 10.0f);
            camera = Mathf::ease(Mathf::Easing::quadratic_in_out, camera, room_camera_target(player->entity->position, current_room).cast_to<float>(), GameProperties::delta_time() * 20.0f);
            // camera = {roundf(camera[0]), roundf(camera[1])};
            // camera = room_camera_target(player->entity->position, current_room);
        }

        // Restart check
        if (player->get_hp() <= 0 && player->entity->is_active)
        {
            player->entity->is_active = false;
            persistent_interactions.clear();
            inventory.clear();
            Composer::create_end_sequence(this, {{0, 0}, ""}, Layer::Draw::overlay);
        }
    }
    else
    {
        int player_room_id = find_in_map("Player");
        player_room_id = player_room_id < 0 ? 0 : player_room_id;
        
        last_room_entities = get_entities();
        move_to_room(player_room_id);
    }

    // Beware that the camera is rounding for the nearest integer to avoid stuttering
    // This can be done because we cannot render half-pixel anyway
    // This needs to be removed if we drop fixed resolution 
    for (const auto& layer : camera_layers)
        layer_transform(layer, LinAlg2D::gen_translation(-roundf(camera[0]), -roundf(camera[1])));
}


void Level::layer_transform(Layer::Draw layer, const auk::Mat3x2& transform)
{
	Scene::layer_transform((int)layer, transform);
}

void Level::render(auk::BatchRenderer& renderer)
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

void Level::add_persistent_interaction(const std::string& id)
{
    persistent_interactions[current_room.id].push_back(id);
}

void Level::clear_persistent_interactions()
{
    persistent_interactions.clear();
}

bool Level::has_persistent_interaction(size_t room_id, const std::string& id)
{
    const auto& vec = persistent_interactions[room_id];
    bool result = false;
    if( std::find(vec.begin(), vec.end(), id) != vec.end() )
    {
        result = true;
    }
    return result;
}