#pragma once

#include "core/ecs.h"
#include "layers.h"

#include <auk.h>

#include <string>
#include <unordered_map>

// This namespace is an object factory for game entities
namespace Lira::Composer { 

    struct ObjectInfo
    {   
        auk::IVec2 pos;
        std::string id;
    };

    // Map

    Entity* create_level(Scene * scene, std::string name, size_t level_n, const auk::IVec2 & position, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_heart(Scene* scene, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_crystal(Scene* scene, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_door(Scene* scene, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_key(Scene* scene, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);

    // Objects

    Entity* create_player(Scene * scene, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_turret(Scene* scene, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_bullet(Scene* scene, const auk::Vec2& direction, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);

    // FX
    
    Entity* create_collect_effect(Scene* scene, const ObjectInfo& obj, const auk::Color& color, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_end_sequence(Scene* scene, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_start_sequence(Scene* scene, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);


    // UI
    Entity* create_hp_bar(Scene* scene, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_minimap(Scene* scene, const ObjectInfo& obj, const Layer::Draw& layer = Layer::Draw::base);

}