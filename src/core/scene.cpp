
#include <tinysdl.h>

#include "entity.h"
#include "scene.h"

using namespace MicroNinja;
using namespace TinySDL;


Entity* Scene::add_entity(const IVec2& pos) {
	auto& e = entities.emplace_back(new Entity(pos, this));
	return e.get();
}











//void Scene::begin() {
//    for(auto& entity: entities) 
//        entity->begin();
//}
//
//void Scene::update() {
//    for(auto& entity: entities) 
//        entity->update();
//}
//
//void Scene::render() {
//    for(auto& entity: entities) 
//        entity->render();
//}