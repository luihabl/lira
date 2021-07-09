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
            virtual void begin();
            virtual void update();
            virtual void render(BatchRenderer & renderer);

            
            Entity* add_entity(const IVec2& pos = { 0, 0 });
            
            template<typename T>
            T* add_component(T&& component, Entity * entity);



            template <typename A, typename... Types>
            static std::unique_ptr<Scene> create_ref(Types ... args) {
                return std::unique_ptr<Scene>(new A(args...));
            }
        
        private:
            Game* game;
            std::list<EntityRef> entities;
            std::list<ComponentRef> components;
    };

    typedef std::unique_ptr<Scene> SceneRef;


    template <typename T>
    inline T* Scene::add_component(T&& component, Entity * entity) {

        T* c = (T*) components.emplace_back(ComponentRef(new T())).get();
        *c = component;

        c->entity = entity;
        entity->components.push_back(c);
        
        return c;
    }

}