
#include "actor.h"
#include "../modules/game.h"

using namespace Lira;
using namespace auk;

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

void Actor::move_x() {
    move_x(velocity[0] * GameProperties::delta_time());
}

void Actor::move_y() {
    move_y(velocity[1] * GameProperties::delta_time());
}

void Actor::move_exact_x(int amount) {
    
    if(collider) {
        int step = Mathf::sign(amount);
        int remaining = amount;

        while (remaining != 0) {

            if(!(collider->check_first(solid_mask, {step, 0}))){
                entity->position[0] += step;
                remaining -= step;
            }
            else {
                if (on_collide_x)
                    on_collide_x(this);
                else
                    stop_x();
                return;
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

            if(!(collider->check_first(solid_mask, {0, step}))){
                entity->position[1] += step;
                remaining -= step;
            }
            else {

                if (on_collide_y)
                    on_collide_y(this);
                else
                    stop_y();
                return;
            }
        }
    }
    else {
        entity->position[1] += amount;
    }
}

void Actor::stop_x()
{
    velocity[0] = 0;
    move_remainder[0] = 0;
}

void Actor::stop_y()
{
    velocity[1] = 0;
    move_remainder[1] = 0;
}

bool Actor::on_ground(int offset) {
    if(collider) {
        return collider->check_first(solid_mask, {0, offset});
    }

    return false;
}

bool Actor::on_wall(int dir) {
    if (collider) {
        return collider->check_first(solid_mask, { dir, 0 });
    }

    return false;
}

void Actor::update() {
    if (auto_update)
    {
        move_x(velocity[0] * GameProperties::delta_time());
        move_y(velocity[1] * GameProperties::delta_time());
    }
}