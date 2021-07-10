
#include <tinysdl.h>

#include "scene.h"
#include "entity.h"
#include "component.h"

using namespace MicroNinja;
using namespace TinySDL;


Entity* Scene::add_entity(const IVec2& pos, int layer) {
	entities.emplace_back(new Entity(pos, this, layer));
	return entities.back().get();
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



}


void Scene::destroy_component(Component * component) {



}