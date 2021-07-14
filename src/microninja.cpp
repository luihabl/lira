#include "microninja.h"
#include "core/scene.h"

#include "level0.h"


void MicroNinjaGame::begin() {

    content.set_folder_name("content");
    content.load_all();

    target = RenderTarget(width, height);

    move_to_scene(SceneRef(new Level0()));

    Graphics::set_blend_mode();
    Game::begin();
}

void MicroNinjaGame::render() {

    target.begin();

        Graphics::viewport(width, height);
        Graphics::clear({0, 0, 0});

        default_shader.use().set_mat4x4("projection", virtual_projection);
    
        Game::render();

        renderer.render();

    target.end();

    Graphics::viewport(window_width, window_height);
    Graphics::clear(Color::black);

    float scale = std::min(window_width / (float)width, window_height / (float)height);
    default_shader.use().set_mat4x4("projection", window_projection);
    renderer.set_texture(&target.tex);
    renderer.draw_tex(target.tex.full_rect, { (float)window_width / 2.0f, (float)window_height / 2.0f }, { scale, -scale }, 0.0f, true);

    renderer.render();
}


