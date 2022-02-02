
#include <tinysdl.h>

#include "ecs.h"

#include <algorithm>

using namespace Lira;
using namespace TinySDL;

Scene::~Scene()
{
	for (Entity* e : entities)
		delete e;
	
	for (Entity* e : entities_cache)
		delete e;
}

void Scene::begin() {
	for (const auto& [id, items] : components)
		for (auto* component : items)
			component->begin();
}

void Scene::update() {

   for (const auto& [id, items] : components)
	  for (size_t i = 0; i < items.size(); i++)
	  {
	      if (items[i]->is_active && items[i]->entity->is_active)
			  items[i]->update();
	  }

   for (auto& c : components_to_remove) 
	   destroy_component(c, c->entity);
   
   for (auto& e : entities_to_remove) 
	   destroy_entity(e);
 
   components_to_remove.clear();
   entities_to_remove.clear();
}

void Scene::render(BatchRenderer & renderer) {

	for (const auto& [id, layer] : render_layers)
	{
		renderer.push_transform(layer.transform);

		for (auto* component : layer.components)
			if (component->is_visible && component->entity->is_visible)
				component->render(renderer);
		
		renderer.pop_transform();	
	}
}

void Scene::queue_remove(Component* component) {
	components_to_remove.push_back(component);
}

void Scene::queue_remove(Entity* entity) {
	entities_to_remove.push_back(entity);
}

Entity* Scene::add_entity(const IVec2& pos, int layer) {
	
	Entity* e = nullptr;

	if (entities_cache.size() > 0)
	{
		e = entities_cache.back();
		entities_cache.pop_back();
		*e = Entity(pos, this, layer);
	}
	else
	{
		e = new Entity(pos, this, layer);
	}
	
	entities.push_back(e);

	return e;
}

void Scene::destroy_entity(Entity * entity) {

	auto& c_list = entity->components;

	for (int i = (int) c_list.size() - 1; i >= 0; i--)
		destroy_component(c_list[i], entity);

	for (size_t i = 0; i < entities.size(); i++)
	{
		if (entities[i] == entity)
		{
			entities[i] = entities.back();
			entities.pop_back();

			if (entities_cache.size() < N_MAX_CACHE)
			{
				entities_cache.push_back(entity);
			}
			else
			{
				delete entity;
			}

			return;
		}
	}
}

void Scene::destroy_component(Component * component, Entity * entity) {

	auto& c_list = entity->components;
	
	for (int i = (int)c_list.size() - 1; i >= 0; i--) {
		if (c_list[i] == component) {

			c_list[i] = c_list.back();
			c_list.pop_back();

			components.release(component);
			render_layers[entity->layer].remove(component);

			if (components_cache.count(component) < N_MAX_CACHE)
			{
				components_cache.add(component);
			}
			else
			{
				delete component;
			}
				
			return;
		}
	}
}

const std::vector<Entity*>& Scene::get_entities()
{ 
	return entities; 
}

void Scene::layer_transform(int layer, const TinySDL::Mat3x2& transform)
{
	render_layers[layer].transform = transform;
}

