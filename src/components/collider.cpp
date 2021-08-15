#include "collider.h"
#include "../core/scene.h"

using namespace MicroNinja;
using namespace TinySDL;

bool Collider::check(Collider & other) {
    auto& a = rect + entity->position;
    auto& b = other.rect + other.entity->position;
    return a.overlaps(b);
}

bool Collider::check_first() {
    auto * components = scene()->get_component_set();

    for(const auto& c: *components) {
	   auto& component = *(c.get());
	   if (component.is_active && type == component.type && (&component) != this) {
			Collider & coll = (Collider&) component;
            if(check(coll)) return true;
       }
   }
    return false;
}

IntRect Collider::scene_rect() {
    return rect + entity->position;
}

void Collider::render(BatchRenderer & renderer) {

    Color c = check_first() ? Color::blue : Color::red;
    renderer.draw_rect_line(scene_rect().cast_to<float>(), c, 1);
}