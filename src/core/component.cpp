#pragma once 

#include "entity.h"
#include "component.h"

using namespace MicroNinja;

int Component::get_layer() { 
    return entity->get_layer(); 
}