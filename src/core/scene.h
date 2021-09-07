#pragma once 

#include <set>
#include <memory>

#include <tinysdl.h>

#include "entity.h"
#include "component.h"
#include "../util/type_table.h"

namespace MicroNinja {

    class Game;
    class Entity;
    class Component;

    class Scene {

        friend class Game;

        public:
            virtual void begin();
            virtual void update();
            virtual void render(TinySDL::BatchRenderer & renderer);

            
            Entity* add_entity(const TinySDL::IVec2& pos = { 0, 0 }, int layer = 0);
            
            template<typename T>
            T* add_component(T&& component, Entity * entity);

            //Queue component destruction for end of frame
            void queue_remove(Component* component);

            //Queue entity destruction for end of frame
            void queue_remove(Entity* entity);

            template <typename T>
            const std::vector<Component*>& get_components() {
                return components.get_group<T>();
            }
            
        private:
            Game* game;

            TypeTable<Component> components;
            std::vector<Entity*> entities;

            std::vector<Component*> components_to_remove;
            std::vector<Entity*> entities_to_remove;

            //Immediately destroys entity
            void destroy_entity(Entity* entity);

            //Immediately destroys component
            void destroy_component(Component* component, Entity* entity);

    };

    typedef std::unique_ptr<Scene> SceneRef;


    template <typename T>
    inline T* Scene::add_component(T&& component, Entity * entity) {

        T* c = new T();
        *c = component;
        c->entity = entity;
        c->type = TinySDL::Type::type_of<T>();

        components.add(c);
        entity->components.push_back(c);
        
        return c;
    }
}