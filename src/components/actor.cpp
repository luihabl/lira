
#include "actor.h"

using namespace MicroNinja;
using namespace TinySDL;

void Actor::move_x(int amount) {
    
    if(collider) {
        int step = Mathf::sign(amount);
        int remaining = amount;

        while (remaining != 0) {

            if(!(collider->check_first())){
                entity->position[0] += step;
                remaining -= step;
            }
            else {
                entity->position[0] -= step;
                break;
            }
        }
    }
    else {
        entity->position[0] += amount;
    }

}

void Actor::move_y(int amount) {

    if(collider) {
        int step = Mathf::sign(amount);
        int remaining = amount;

        while (remaining != 0) {

            if(!(collider->check_first())){
                entity->position[1] += step;
                remaining -= step;
            }
            else {
                entity->position[1] -= step;
                break;
            }
        }
    }
    else {
        entity->position[1] += amount;
    }

}