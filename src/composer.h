#pragma once

#include "core/ecs.h"
#include "core/ecs.h"

#include <tinysdl.h>

#include <string>

// This namespace is an object factory for game entities
namespace Lira::Composer { 
    
    Entity* create_level(Scene * scene, std::string name, size_t level_n, const TinySDL::IVec2 & position, const int layer = 0);
    Entity* create_player(Scene * scene, const TinySDL::IVec2 & position, const int layer = 0);
    Entity* create_turret(Scene* scene, const TinySDL::IVec2& position, const int layer = 0);
    Entity* create_bullet(Scene* scene, const TinySDL::Vec2& direction, const TinySDL::IVec2& position, const int layer = 0);

}