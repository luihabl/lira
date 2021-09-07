#pragma once 

#include "../core/component.h"
#include <tinysdl.h>

#include <functional>

namespace MicroNinja {

    class ColliderGrid;

    class Collider : public Component {
    
    public:
        Collider() = default;
        Collider(TinySDL::IntRect r) : rect(r) {
            is_visible = false;
        } 

        bool check(Collider & other, const TinySDL::IVec2 & offset = TinySDL::IVec2::zeros);
        bool check(ColliderGrid & other, const TinySDL::IVec2 & offset = TinySDL::IVec2::zeros);

        bool check_first(const TinySDL::IVec2 & offset = TinySDL::IVec2::zeros);

        void render(TinySDL::BatchRenderer & renderer) override; 

        std::function<void(const Component&)> on_collide = nullptr;

    private:

        TinySDL::IntRect rect;
        TinySDL::IntRect scene_rect();
    };
}