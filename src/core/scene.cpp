
#include <tinysdl.h>

#include "scene.h"
#include "entity.h"
#include "component.h"

#include <algorithm>

using namespace MicroNinja;
using namespace TinySDL;


Entity* Scene::add_entity(const IVec2& pos, int layer) {
	auto& e = entities.insert(EntityRef(new Entity(pos, this, layer)));
	return e->get();
}

void Scene::begin() {
	for (const auto& [id, items] : components)
		for (auto* component : items)
			component->begin();
}

void Scene::update() {

   for (const auto& [id, items] : components)
	   for (auto* component : items) 
	   {
		   if (component->is_active && component->entity->is_active)
			   component->update();
	   }

   for (auto& c : components_to_remove) 
	   destroy_component(c, c->entity);
   
   for (auto& e : entities_to_remove) 
	   destroy_entity(e);
 
   components_to_remove.clear();
   entities_to_remove.clear();

}

void Scene::render(BatchRenderer & renderer) {

	std::vector<Component*> render_list;
	
	for (const auto& [id, items] : components)
	   for (auto* component : items)
	   {
		   if (component->is_visible && component->entity->is_visible)
			   render_list.push_back(component);
	   }


	std::sort(render_list.begin(), render_list.end(), [](Component* a, Component* b)->bool {
		return a->get_layer() < b->get_layer();
	});

	for (auto* item : render_list)
		item->render(renderer);

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

			c_list[i] = c_list.back();
			c_list.pop_back();

			//Transfer this to cache instead of deleting?
			components.erase(component);

			return;
		}
	}
	
	
}

