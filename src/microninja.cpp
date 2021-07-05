#include "microninja.h"
#include "core/scene.h"

#include "level0.h"

void MicroNinjaGame::begin() {

    content.set_folder_name("content");
    content.load_all();

    target = RenderTarget(width, height);

    set_scene<Level0>(); //unique_ptr does not allow object copying

    Graphics::set_blend_mode();
    Game::begin();
}

void MicroNinjaGame::render() {

    target.begin();

    Graphics::viewport(width, height);
    Graphics::clear({ 10, 50, 115 });

    default_shader.use().set_mat4x4("projection", virtual_projection);


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


    renderer.render();

    target.end();

    Graphics::viewport(window_width, window_height);
    Graphics::clear(Color::black);

    float scale = std::min(window_width / (float)width, window_height / (float)height);
    default_shader.use().set_mat4x4("projection", window_projection);
    renderer.set_texture(&target.tex);
    renderer.draw_tex(target.tex.full_rect, { (float)window_width / 2.0f, (float)window_height / 2.0f }, { scale, -scale }, 0.0f, true);

    renderer.render();

    Game::render();
}


