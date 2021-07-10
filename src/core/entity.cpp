#include "entity.h"
#include "scene.h"

using namespace MicroNinja;

void Entity::destroy() { 
    scene->queue_remove(this); 
}