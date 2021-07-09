#pragma once 

#include <list>
#include <tinysdl.h>
#include <memory>

#include "entity.h"
#include "component.h"

using namespace TinySDL;

namespace MicroNinja {

    // template <typename T>
    // class RefType {
    //     public:
    //         template <typename A, typename... Types>
    //         static std::unique_ptr<T> create(Types ... args) {
    //             return std::unique_ptr<T>(new A(args...));
    //         }

    //         typedef std::unique_ptr<T> Ref;
    // };


    class Game;

    class Scene {

        friend class Game;

        public:
            virtual void begin() {};
            virtual void update() {};
            virtual void render(BatchRenderer & renderer) {};

            

            Entity* add_entity(const IVec2& pos = { 0, 0 });

            template <typename A, typename... Types>
            static std::unique_ptr<Scene> create_ref(Types ... args) {
                return std::unique_ptr<Scene>(new A(args...));
            }
        
        private:
            Game* game;
        
        protected:
            std::list<EntityRef> entities;
            std::list<ComponentRef> components;
    };

    typedef std::unique_ptr<Scene> SceneRef;


}