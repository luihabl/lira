#pragma once 

#include <list>
#include <set>
#include <memory>

#include <tinysdl.h>

#include "entity.h"
#include "component.h"

using namespace TinySDL;

namespace MicroNinja {

    class Game;
    class Entity;
    class Component;

    class Scene {

        friend class Game;

        public:
            virtual void begin();
            virtual void update();
            virtual void render(BatchRenderer & renderer);

            
            Entity* add_entity(const IVec2& pos = { 0, 0 }, int layer = 0);
            
            template<typename T>
            T* add_component(T&& component, Entity * entity);

            void destroy_entity(Entity * entity);
            void destroy_component(Component * component);

            template <typename A, typename... Types>
            static std::unique_ptr<Scene> create_ref(Types ... args) {
                return std::unique_ptr<Scene>(new A(args...));
            }
        
        private:
            Game* game;
            std::list<EntityRef> entities;

            struct LayerComparison {
                bool operator()(const ComponentRef & a, const ComponentRef & b) const {
                    return a->get_layer() < b->get_layer();
                } 
            };

            // Note: 
            // multiset is used here to keep the layers ordered,
            // however this is not necesseraly the fastest way to do it. 
            // Maybe this will change in the future.
            std::multiset<ComponentRef, LayerComparison> components;

    };

    typedef std::unique_ptr<Scene> SceneRef;


    template <typename T>
    inline T* Scene::add_component(T&& component, Entity * entity) {

        ComponentRef new_component{new T()};
        T* c = (T*) new_component.get();
        *c = component;
        c->entity = entity;

        auto & c_it = components.insert(std::move(new_component));
        entity->components.push_back(c);
        
        return c;
    }

}