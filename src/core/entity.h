#pragma once 

#include <vector>
#include <tinysdl.h>

#include <memory>

using namespace TinySDL;

namespace MicroNinja
{
    class Scene;
    class Component;

    class Entity {

    public:
        Entity(const IVec2& pos, Scene * s) : position(pos), scene(s) {};

        IVec2 position;
        Scene* scene;
        std::vector<Component *> components;

        template <typename T>
        T* add_component(T&& component = T());
    };

    typedef std::unique_ptr<Entity> EntityRef;


    template <typename T>
    T* add_component(T&& component) {

    }


} 
