#pragma once 

#include <vector>

#include "component.h"

namespace MicroNinja
{
    class Entity {
        public:
            virtual void begin() {};
            virtual void update() {};
            virtual void render() {};

        protected:
            std::vector<Component> components;

    };



} 
