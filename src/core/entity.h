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

        friend class Scene;

    public:
        Entity(const IVec2& pos, Scene * s) : position(pos), scene(s) {};

        IVec2 position;

        bool is_active = true;
        bool is_visible = true;
        
        template <typename T>
        T* add_component(T&& component = T());

    private:
        Scene* scene;
        std::vector<Component *> components;
    };

    typedef std::unique_ptr<Entity> EntityRef;


    template <typename T>
    T* add_component(T&& component) {

    }


} 
