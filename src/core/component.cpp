#include "scene.h"
#include "entity.h"
#include "component.h"

using namespace Lira;

int Component::get_layer() { 
    return entity->get_layer(); 
}

void Component::destroy() {
    scene()->queue_remove(this);
}