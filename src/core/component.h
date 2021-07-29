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

        virtual void begin() {};
        virtual void update() {};
        virtual void render(TinySDL::BatchRenderer & renderer) {};

        int get_layer();

    };

    typedef std::unique_ptr<Component> ComponentRef;
}