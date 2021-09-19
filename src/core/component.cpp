#include "scene.h"
#include "entity.h"
#include "component.h"

using namespace MicroNinja;

int Component::get_layer() { 
    return entity->get_layer(); 
}

void Component::destroy() {
    scene()->queue_remove(this);
}