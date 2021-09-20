#pragma once

#include "core/entity.h"
#include "core/scene.h"

#include <tinysdl.h>

#include <string>

// This namespace is an object factory for game entities
namespace MicroNinja::Composer { 

    Entity * create_map(Scene * scene, std::string name, const TinySDL::IVec2 & position, const int layer = 0);
    Entity * create_player(Scene * scene, const TinySDL::IVec2 & position, const int layer = 0);
    Entity * create_turret(Scene* scene, const TinySDL::IVec2& position, const int layer = 0);

}