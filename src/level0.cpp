
#include <tinysdl.h>

#include "level0.h" 
#include "tinysdl.h"
#include "assets/content.h"
#include "modules/game.h"

using namespace MicroNinja;
using namespace TinySDL;


void Level0::begin() {
    // Add entities and components here...

    auto* e = add_entity({ 10, 2 });

}


void Level0::update() {
    // Add logic here
    // Input should be captured in the main game class
    

}

void Level0::render(BatchRenderer & renderer) {

    renderer.set_texture(Content::find_tex("tilesets/Game Boy GBC - Pokemon Gold Silver - Cinnabar Island"));
    renderer.draw_tex({ 0.0f, 0.0f, 16.0f, 16.0f }, { 20.0f, 0.0f, 16.0f, 16.0f }, 0.5f, false);
    renderer.draw_tex({ 16.0f, 16.0f, 16.0f, 16.0f }, { 100.0f + 20.0f, 20.0f, 16.0f, 16.0f }, 0.0f, false);

    renderer.draw_rect_fill({ 120, 60, 30, 60 }, Color::blue);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            renderer.draw_tex({ 16.0f * (float)i, 16.0f * (float)j, 16.0f, 16.0f }, { (float)i * 16.f, (float)j * 16.f });
        }
    }

    renderer.draw_arc_line({ 80, 80 }, 40, 0.5f, 0.2f, 3, Color::red, 40);

    renderer.set_texture(Content::find_tex("sprites/dude_transparent"));
    //renderer.set_texture(&dude);
    renderer.draw_tex({ 0, 0, 16, 16 }, { 20, 20 });
    renderer.set_texture(Content::find_tex("tilesets/Game Boy GBC - Pokemon Gold Silver - Cinnabar Island"));
    renderer.draw_tex({ 0, 0, 80, 80 }, { 30, 30 });
}