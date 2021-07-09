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

            
            Entity* add_entity(const IVec2& pos = { 0, 0 }, int layer = 0);
            
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

        // T* c;
        
        // int entity_layer = entity->get_layer();
        // std::list<ComponentRef>::iterator it = components.begin();
        // for ( = ; it != components.end(); ++it) {

        //     if (it == components.end()) {

        //     }
        // }

        // do {

        //     if (it == components.end()) {
        //         Log::debug("asdasd");
        //         c = (T*) components.emplace(it, ComponentRef(new T()))->get();
        //         break;
        //     }
        //     else if (entity_layer > it->get()->get_layer())
        //     {
        //         c = (T*) components.emplace(it, ComponentRef(new T()))->get();
        //         break;
        //     }
            
        //     it++;
        // } while (it != components.end());


        T* c = (T*) components.emplace_back(ComponentRef(new T())).get();




        *c = component;

        c->entity = entity;
        entity->components.push_back(c);
        
        return c;
    }

}