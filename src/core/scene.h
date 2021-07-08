#pragma once 

#include <list>
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

            Entity* add_entity(const IVec2& pos = { 0, 0 });
            

        protected:
            std::list<EntityRef> entities;
            std::list<ComponentRef> components;
    };

    typedef std::unique_ptr<Scene> SceneRef;

}