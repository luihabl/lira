#pragma once 

#include <memory>
#include <utility>
#include <vector>

#include <tinysdl.h>

using namespace TinySDL;

namespace MicroNinja
{
    class Scene;
    class Component;

    class Entity {

        friend class Scene;

    public:
        Entity(const IVec2& pos, Scene * s, int layer = 0) : position(pos), scene(s), layer(layer) {};

        IVec2 position;

        bool is_active = true;
        bool is_visible = true;
        
        template <typename T>
        T* add_component(T&& component = T());

        int get_layer(){ return layer; }

        void destroy();


    private:
        int layer = 0;
        Scene* scene;
        std::vector<Component *> components;
    };

    typedef std::unique_ptr<Entity> EntityRef;


    template <typename T>
    T* Entity::add_component(T&& component) {
        return scene->add_component(std::forward<T>(component), this);
    }


} 
