#pragma once 

#include <set>
#include <memory>

#include <tinysdl.h>

#include "entity.h"
#include "component.h"
#include "../util/type_table.h"

namespace Lira {

    class Game;
    class Entity;
    class Component;

    constexpr size_t N_MAX_CACHE = 512;

    class Scene {

        friend class Game;

        public:
            virtual void begin();
            virtual void update();
            virtual void render(TinySDL::BatchRenderer & renderer);

            virtual ~Scene();
            
            Entity* add_entity(const TinySDL::IVec2& pos = { 0, 0 }, int layer = 0);
            
            template<typename T>
            T* add_component(T&& component, Entity * entity);

            //Queue component destruction for end of frame
            void queue_remove(Component* component);

            //Queue entity destruction for end of frame
            void queue_remove(Entity* entity);

            template <typename T>
            const std::vector<Component*>& get_components();

            template <typename T>
            size_t components_count();

            template <typename T>
            const T* get_first();

            const std::vector<Entity*>& get_entities();

            
        private:
            Game* game;

            TypeTable<Component> components;
            TypeTable<Component> components_cache;

            std::vector<Entity*> entities;
            std::vector<Entity*> entities_cache;

            std::vector<Component*> components_to_remove;
            std::vector<Entity*> entities_to_remove;

            std::vector<Component*> components_to_add;

            //Immediately destroys entity
            void destroy_entity(Entity* entity);

            //Immediately destroys component
            void destroy_component(Component* component, Entity* entity);

            //Immediately adds component
            template <typename T>
            void insert_component(T* component);

    };

    typedef std::unique_ptr<Scene> SceneRef;


    template <typename T>
    const std::vector<Component*>& Scene::get_components()
    {
        return components.get_group<T>();
    }

    template <typename T>
    size_t Scene::components_count()
    {       
        return components.count<T>();
    }

    template <typename T>
    const T* Scene::get_first()
    {
        const auto& group = components.get_group<T>();
        if(group.size() > 0)
            return (T*) group[0];
        
        return nullptr;
    }

    template <typename T>
    inline T* Scene::add_component(T&& component, Entity * entity) 
    {
        T* c = nullptr;

        if (components_cache.count<T>() > 0)
        {
            c = components_cache.pop<T>();
        }
        else 
        {
            c = new T();
        }
            
        *c = component;
        c->entity = entity;
        c->type = TinySDL::Type::type_of<T>();

        components_to_add.push_back(c);

        return c;
    }

    template <typename T>
    inline void Scene::insert_component(T* c)
    {
        components.add(c);
        c->entity->components.push_back(c);
        
        c->begin();
    }
}