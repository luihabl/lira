#pragma once 

#include <vector>
#include <tinysdl.h>

#include <memory>

namespace MicroNinja
{
    class Scene;
    class Component;

    class Entity {

        TinySDL::IVec2 position;

        Scene* scene;

        std::vector<Component *> components;

    };

    typedef std::unique_ptr<Entity> EntityRef;

} 
