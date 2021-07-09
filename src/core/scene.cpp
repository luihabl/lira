
#include <tinysdl.h>

#include "entity.h"
#include "scene.h"

using namespace MicroNinja;
using namespace TinySDL;


Entity* Scene::add_entity(const IVec2& pos, int layer) {
	auto& e = entities.emplace_back(new Entity(pos, this, layer));
	return e.get();
}

void Scene::begin() {
   for(auto& component: components) 
       component.get()->begin();
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