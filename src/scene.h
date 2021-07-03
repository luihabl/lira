#pragma once 

#include <vector>

#include "entity.h"

namespace MicroNinja {
    
    class Scene {
        public:
            virtual void begin();
            virtual void update();
            virtual void render();

        protected:
            std::vector<Entity> entities;

    };
}