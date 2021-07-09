#pragma once

#include <memory>

#include <tinysdl.h>
#include "entity.h"

using namespace TinySDL;

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
        virtual void render(BatchRenderer & renderer) {};

    };

    typedef std::unique_ptr<Component> ComponentRef;
}