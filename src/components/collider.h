#pragma once 

#include "../core/ecs.h"
#include <auk.h>

#include <functional>

namespace Lira {

    class ColliderGrid;

    class Collider : public Component {
    
    public:
        Collider() = default;
        Collider(auk::IntRect r) : rect(r) {
            is_visible = false;
        } 

        bool check(Collider & other, const auk::IVec2 & offset = auk::IVec2::zeros);
        bool check(ColliderGrid & other, const auk::IVec2 & offset = auk::IVec2::zeros);

        bool check_first(uint32_t mask = -1, const auk::IVec2& offset = auk::IVec2::zeros);

        void render(auk::BatchRenderer & renderer) override; 

        uint32_t layer = -1;

    private:

        auk::IntRect rect;
        auk::IntRect scene_rect();

        template <typename T>
        bool check_first_t(uint32_t mask, const auk::IVec2& offset)
        {
            auto& colliders = scene()->get_components<T>();

            for (const auto* c : colliders) {

                T& coll = (T&)*c;
                if (coll.is_active && c != (Component*) this && (coll.layer & mask)) {
                    if (check(coll, offset))
                    {
                        return true;
                    }
                }
            }

            return false;
        }


    };
}