#pragma once 

#include <vector>

#include "entity.h"
#include "component.h"
#include "../static/ref.h"

namespace MicroNinja {
    
    class Game;

    class Scene {
        public:
            virtual void begin() {};
            virtual void update() {};
            virtual void render() {};

            Game* game;

        protected:
            std::vector<EntityRef> entities;
            std::vector<ComponentRef> components;
    };

    typedef Ref::Unique<Scene> SceneRef;
}