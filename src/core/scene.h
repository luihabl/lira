#pragma once 

#include <vector>
#include <tinysdl.h>

#include "entity.h"
#include "component.h"

using namespace TinySDL;

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

    typedef std::unique_ptr<Scene> SceneRef;
}