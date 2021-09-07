#include "collider.h"
#include "collider_grid.h"
#include "../core/scene.h"

#include <cmath>

using namespace MicroNinja;
using namespace TinySDL;

bool Collider::check(Collider & other, const IVec2 & offset) {
    auto& a = rect + entity->position + offset;
    auto& b = other.rect + other.entity->position;
    return a.overlaps(b);
}

bool Collider::check(ColliderGrid & other, const IVec2 & offset) {
    
    const auto& r = rect + entity->position + offset;
    const auto& [w, h] = other.get_cell_size();


    int x_0 = (int) ((float) r.x / (float) w);
    int x_1 = (int) ((float) (r.x + r.w - 1) / (float) w);
    
    int y_0 = (int) ((float) r.y / (float) h);
    int y_1 = (int) ((float) (r.y + r.h - 1) / (float) h);

    for(int y = y_0; y <= y_1; y++) {
        for(int x = x_0; x <= x_1; x++) {
            if (other.has_cell(x, y))
                return true;
        }
    }

    return false;
}


bool Collider::check_first(const IVec2 & offset) {

    auto& colliders = scene()->get_components<Collider>();

    for(const auto* c: colliders) {
	   if (c->is_active && (Collider*)c != this) {
			Collider & coll = (Collider&) c;
            if(check(coll, offset)) return true;
       }
    }

    auto& grid_colliders = scene()->get_components<ColliderGrid>();

    for (const auto* c : grid_colliders) {
        if (c->is_active) {
            ColliderGrid& coll = (ColliderGrid&) *c;
            if (check(coll, offset)) return true;
        }
    }

    return false;
}

IntRect Collider::scene_rect() {
    return rect + entity->position;
}

void Collider::render(BatchRenderer & renderer) {
    renderer.draw_rect_line(scene_rect().cast_to<float>(), Color::red, 1);
}