#include "collider.h"
#include "collider_grid.h"
#include "../core/ecs.h"

#include <cmath>

using namespace Lira;
using namespace auk;

bool Collider::check(Collider & other, const IVec2 & offset) {
    const auto& a = rect + entity->position + offset;
    const auto& b = other.rect + other.entity->position;
    return a.overlaps(b);
}

bool Collider::check(ColliderGrid & other, const IVec2 & offset) {
    
    const auto& r = rect + (offset + entity->position - other.entity->position);
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


bool Collider::check_first(uint32_t mask, const IVec2& offset) {

    if (check_first_t<Collider>(mask, offset))
        return true;

    if (check_first_t<ColliderGrid>(mask, offset))
        return true;

    return false;
}

IntRect Collider::scene_rect() {
    return rect + entity->position;
}

void Collider::render(BatchRenderer & renderer) {
    renderer.draw_rect_line(scene_rect().cast<float>(), Color::red, 1);
}