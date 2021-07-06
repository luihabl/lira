#pragma once 

#include <vector>
#include <tinysdl.h>

#include "../static/ref.h"

namespace MicroNinja
{
    class Scene;
    class Component;

    class Entity {

        TinySDL::IVec2 position;

        Scene* scene;

        std::vector<Component *> components;

    };

    typedef Ref::Unique<Entity> EntityRef;

} 
