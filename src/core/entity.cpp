#include "entity.h"
#include "scene.h"

using namespace MicroNinja;

void Entity::destroy() { 
    scene->destroy_entity(this); 
}