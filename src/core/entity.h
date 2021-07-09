#pragma once 

#include <vector>
#include <tinysdl.h>

#include <memory>
#include <utility>

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
    T* Entity::add_component(T&& component) {
        return scene->add_component(std::forward<T>(component), this);
    }


} 
