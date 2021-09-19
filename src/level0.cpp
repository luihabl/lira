
#include <tinysdl.h>

#include "level0.h" 
#include "composer.h"
#include "modules/game.h"
#include "input/input.h"

#include "components/collider.h"
#include "components/collider_grid.h"

using namespace MicroNinja;
using namespace TinySDL;


void Level0::begin() {


    auto* map = Composer::create_map(this, "tilemaps/map0", {0, 0});
    //auto* player = Composer::create_player(this, "sprites/player", {0, 0}, 1);
    auto* turret = Composer::create_turret(this, { 16 * 15, 16 * 8 });



    Scene::begin();
}


void Level0::update() {
    if (Input::just_pressed(Key::F1))
        render_colliders = !render_colliders;

    Scene::update();
}

void Level0::render(TinySDL::BatchRenderer& renderer)
{
    Graphics::clear({ 31, 15, 0 });

    Scene::render(renderer);
    
    if (render_colliders)
    {
        auto& colliders = get_components<Collider>();
        auto& grid_colliders = get_components<ColliderGrid>();

        for (auto* c : colliders)
            c->render(renderer);

        for (auto* c : grid_colliders)
            c->render(renderer);
    }

}
