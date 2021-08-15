#pragma once 

#include <memory>
#include <utility>
#include <vector>

#include <tinysdl.h>

namespace MicroNinja
{
    class Scene;
    class Component;

    class Entity {

        friend class Scene;

    public:
        Entity(const TinySDL::IVec2& pos, Scene * s, int layer = 0) : position(pos), scene(s), layer(layer) {};

        TinySDL::IVec2 position;

        bool is_active = true;
        bool is_visible = true;
        
        template <typename T>
        T* add_component(T&& component = T());

        template <typename T>
        T* get_component();

        int get_layer(){ return layer; }

        void destroy();

        Scene* get_scene() { return scene; }

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

    template <typename T>
    T* Entity::get_component() {
        for (auto* c : components) {
            if (c->type == TinySDL::Type::type_of<T>()) {
                return (T*) c;
            }
        }
        
        return nullptr;
    }


} 
