#pragma once

#include "entity.h"

#include <memory>
#include <tinysdl.h>

namespace MicroNinja {

    class Scene;
    class Entity;

    class Component {

        friend class Scene;
        friend class Entity;

    public:
        bool is_active = true;
        bool is_visible = true;

        Entity* entity;
        Scene* scene() { return entity->get_scene(); }

        TinySDL::Type type;

        virtual void begin() {};
        virtual void update() {};
        virtual void render(TinySDL::BatchRenderer & renderer) {};

        int get_layer();
        
        template <typename T>
        T* get_sibling() {
            return entity->get_component<T>();
        }
    };

    typedef std::unique_ptr<Component> ComponentRef;
}