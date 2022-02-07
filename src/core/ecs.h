#pragma once

#include <memory>
#include <utility>
#include <vector>
#include <algorithm>
#include <set>
#include <map>

#include "../util/type_table.h"

#include <tinysdl.h>

namespace Lira
{

    class Game;
    class Entity;
    class Component;
    class Scene;

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

            void layer_transform(int layer, const TinySDL::Mat3x2& transform);
            
        private:
            Game* game;

            TypeTable<Component> components;
            TypeTable<Component> components_cache;

            std::vector<Entity*> entities;
            std::vector<Entity*> entities_cache;

            std::vector<Component*> components_to_remove;
            std::vector<Entity*> entities_to_remove;

            struct RenderLayer
            {
                std::vector<Component*> components;
                TinySDL::Mat3x2 transform = TinySDL::Mat3x2::identity;
                
                void add(Component* c) { components.push_back(c); }

                void remove(Component* c)
                {
                    for (size_t i = 0; i < components.size(); i++)
                        if (components[i] == c)
                        {
                            components[i] = components.back();
                            components.pop_back();
                            break;
                        }
                }
            };

            std::map<int, RenderLayer> render_layers;

            //Immediately destroys entity
            void destroy_entity(Entity* entity);

            //Immediately destroys component
            void destroy_component(Component* component, Entity* entity);
    };

    class Entity {

        friend class Scene;

    public:
        Entity(const TinySDL::IVec2& pos, Scene * s, int layer = 0) : position(pos), scene(s), layer(layer) {};

        TinySDL::IVec2 position;

        bool is_active = true;
        bool is_visible = true;
        
        template <typename T>
        T* add_component(T&& component = T());

        template <typename T>
        T* get_component() const;

        int get_layer(){ return layer; }

        void destroy();

        Scene* get_scene() { return scene; }

    private:
        int layer = 0;
        Scene* scene;
        std::vector<Component *> components;
    };

    class Component {

        friend class Scene;
        friend class Entity;

        public:
        virtual ~Component() {};

        bool is_active = true;
        bool is_visible = true;

        Entity* entity;
        Scene* scene() { return entity->get_scene(); }

        TinySDL::Type type;

        virtual void begin() {};
        virtual void update() {};
        virtual void render([[maybe_unused]] TinySDL::BatchRenderer & renderer) {};

        int get_layer();
        
        template <typename T>
        T* get_sibling() {
            return entity->get_component<T>();
        }

        void destroy();
    };

    typedef std::unique_ptr<Scene> SceneRef;
    typedef std::unique_ptr<Entity> EntityRef;

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
        
        components.add(c);
        c->entity->components.push_back(c);
        render_layers[entity->layer].add(c);

        c->begin();

        return c;
    }

    template <typename T>
    T* Entity::add_component(T&& component) {
        return scene->add_component(std::forward<T>(component), this);
    }

    template <typename T>
    T* Entity::get_component() const {
        for (auto* c : components) {
            if (c->type == TinySDL::Type::type_of<T>()) {
                return (T*) c;
            }
        }
        
        return nullptr;
    } 
}