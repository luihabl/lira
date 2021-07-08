#pragma once 

#include <list>
#include <tinysdl.h>
#include <memory>

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
            
            template <typename T, typename... Types>
            static std::unique_ptr<Scene> create(Types ... args) {
                return std::unique_ptr<Scene>(new T(args...));
            }


        protected:
            std::list<EntityRef> entities;
            std::list<ComponentRef> components;
    };

    typedef std::unique_ptr<Scene> SceneRef;

}