#pragma once

#include <memory>

namespace MicroNinja {

    class Scene;
    class Entity;

    class Component {

        bool active = true;
        bool visible = true;

        Scene* scene;
        Entity* entity;

        virtual void begin() {};
        virtual void update() {};
        virtual void render() {};

    };

    typedef std::unique_ptr<Component> ComponentRef;
}