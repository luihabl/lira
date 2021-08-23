
#include "actor.h"
#include "../modules/game.h"

using namespace MicroNinja;
using namespace TinySDL;

void Actor::move_x(float amount) {
    move_remainder[0] += (float) amount;
    int move = (int) round(move_remainder[0]);

    if (move != 0) {
        move_remainder[0] -= (float) move;
        move_exact_x(move);
    }
}

void Actor::move_y(float amount) {
    move_remainder[1] += (float) amount;
    int move = (int) round(move_remainder[1]);

    if (move != 0) {
        move_remainder[1] -= (float) move;
        move_exact_y(move);
    }
}

void Actor::move_exact_x(int amount) {
    
    if(collider) {
        int step = Mathf::sign(amount);
        int remaining = amount;

        while (remaining != 0) {

            if(!(collider->check_first({step, 0}))){
                entity->position[0] += step;
                remaining -= step;
            }
            else {
                velocity[0] = 0;
                move_remainder[0] = 0;
                break;
            }
        }
    }
    else {
        entity->position[0] += amount;
    }

}

void Actor::move_exact_y(int amount) {

    if(collider) {
        int step = Mathf::sign(amount);
        int remaining = amount;

        while (remaining != 0) {

            if(!(collider->check_first({0, step}))){
                entity->position[1] += step;
                remaining -= step;
            }
            else {
                velocity[1] = 0;
                move_remainder[1] = 0;
                break;
            }
        }
    }
    else {
        entity->position[1] += amount;
    }
}

bool Actor::on_ground(int offset) {
    if(collider) {
        return collider->check_first({0, offset});
    }

    return false;
}



void Actor::update() {

    move_x(velocity[0] * GameProperties::delta_time());
    move_y(velocity[1] * GameProperties::delta_time());

}