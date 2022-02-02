#include "ecs.h"

using namespace Lira;

void Entity::destroy() { 
    scene->queue_remove(this); 
}