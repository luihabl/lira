
#include <tinysdl.h>

#include "scene.h"
#include "entity.h"
#include "component.h"

using namespace MicroNinja;
using namespace TinySDL;


Entity* Scene::add_entity(const IVec2& pos, int layer) {

	EntityRef new_entity{ new Entity(pos, this, layer) };

	auto& e = entities.insert(std::move(new_entity));
	return e->get();
}

void Scene::begin() {
   for(auto& c: components) 
       c.get()->begin();
}

void Scene::update() {
   for(const auto& c: components) {
	   auto& component = *(c.get());
	   if (component.is_active && component.entity->is_active)
			component.update();
   }      
}

void Scene::render(BatchRenderer & renderer) {
   for(const auto& c: components) {
	   auto& component = *(c.get());
	   if (component.is_visible && component.entity->is_visible)
			component.render(renderer);
   }   
}

void Scene::destroy_entity(Entity * entity) {

	auto& c_list = entity->components;

	for (int i = (int) c_list.size() - 1; i >= 0; i--)
		destroy_component(c_list[i], entity);

	entities.erase(*find_ref(entities, entity));
}


void Scene::destroy_component(Component * component, Entity * entity) {

	auto& c_list = entity->components;
	
	for (int i = (int)c_list.size() - 1; i >= 0; i--) {
		if (c_list[i] == component) {
			c_list.erase(c_list.begin() + i);
			break;
		}
	}
	
	//Transfer this to cache instead of deleting?
	components.erase(*find_ref(components, component));
}