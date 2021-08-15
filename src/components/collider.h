#pragma once 

#include "../core/component.h"
#include <tinysdl.h>

namespace MicroNinja {

    class Collider : public Component {
    
    public:
        Collider() = default;
        Collider(TinySDL::IntRect r) : rect(r) {} 

        bool check(Collider & other);
        bool check_first();

        void render(TinySDL::BatchRenderer & renderer) override; 

    private:

        TinySDL::IntRect rect;
        TinySDL::IntRect scene_rect();
    };
}