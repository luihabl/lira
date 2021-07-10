
#include <tinysdl.h>

#include "scene.h"
#include "entity.h"
#include "component.h"

using namespace MicroNinja;
using namespace TinySDL;


Entity* Scene::add_entity(const IVec2& pos, int layer) {
	auto& e = entities.insert(EntityRef(new Entity(pos, this, layer)));
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

   for (auto& c : components_to_remove) 
	   destroy_component(c, c->entity);
   
   for (auto& e : entities_to_remove) 
	   destroy_entity(e);
 
   components_to_remove.clear();
   entities_to_remove.clear();

}

void Scene::render(BatchRenderer & renderer) {
   for(const auto& c: components) {
	   auto& component = *(c.get());
	   if (component.is_visible && component.entity->is_visible)
			component.render(renderer);
   }   
}

void Scene::queue_remove(Component* component) {
	components_to_remove.push_back(component);
}

void Scene::queue_remove(Entity* entity) {
	entities_to_remove.push_back(entity);
}

void Scene::destroy_entity(Entity * entity) {

	auto& c_list = entity->components;

	for (int i = (int) c_list.size() - 1; i >= 0; i--)
		destroy_component(c_list[i], entity);

	entities.erase(*find_ref(entities, entity));
	entity = nullptr;
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

