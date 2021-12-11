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

        bool check_first(const TinySDL::IVec2 & offset = TinySDL::IVec2::zeros, uint32_t mask = -1);

        void render(TinySDL::BatchRenderer & renderer) override; 

        std::function<void(const Component&)> on_collide = nullptr;
        uint32_t layer = -1;

    private:

        TinySDL::IntRect rect;
        TinySDL::IntRect scene_rect();

        template <typename T>
        bool check_first_t(const TinySDL::IVec2& offset, uint32_t mask)
        {
            auto& colliders = scene()->get_components<T>();

            for (const auto* c : colliders) {

                T& coll = (T&)*c;
                if (coll.is_active && c != (Component*) this && (coll.layer & mask)) {
                    if (check(coll, offset))
                    {
                        if (on_collide) on_collide(coll);
                        return true;
                    }
                }
            }

            return false;
        }


    };
}