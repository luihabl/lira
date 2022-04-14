#pragma once

#include "core/ecs.h"
#include "layers.h"

#include <tinysdl.h>

#include <string>

// This namespace is an object factory for game entities
namespace Lira::Composer { 
    
    Entity* create_level(Scene * scene, std::string name, size_t level_n, const TinySDL::IVec2 & position, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_player(Scene * scene, const TinySDL::IVec2 & position, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_turret(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_bullet(Scene* scene, const TinySDL::Vec2& direction, const TinySDL::IVec2& position, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_heart(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_crystal(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer = Layer::Draw::base);

    // FX
    Entity* create_collect_effect(Scene* scene, const TinySDL::IVec2& position, const TinySDL::Color& color, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_end_sequence(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_start_sequence(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer = Layer::Draw::base);


    // UI
    Entity* create_hp_bar(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer = Layer::Draw::base);
    Entity* create_minimap(Scene* scene, const TinySDL::IVec2& position, const Layer::Draw& layer = Layer::Draw::base);

}