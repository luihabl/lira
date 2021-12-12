#include "collider.h"
#include "collider_grid.h"
#include "../core/scene.h"

#include <cmath>

using namespace Lira;
using namespace TinySDL;

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


// Add the bitmask support. check first can have two versions 
// check_first(uint32_t layer, const IVec2 & offset) and 
// check_first(const IVec2 & offset). The second one will compare
// with its own layer. For damage is it better to use two different colliders
// or specify the specific layer for each process (move compares to solid, 
// damage compares to enemy, etc)?
bool Collider::check_first(const IVec2 & offset, uint32_t mask) {

    if (
            check_first_t<Collider>(offset, mask) || 
            check_first_t<ColliderGrid>(offset, mask)
        )
    {
        return true;
    }

    return false;
}

IntRect Collider::scene_rect() {
    return rect + entity->position;
}

void Collider::render(BatchRenderer & renderer) {
    renderer.draw_rect_line(scene_rect().cast<float>(), Color::red, 1);
}