#include <tinysdl.h>

#include <chrono>
#include <thread>

#include "game.h"

using namespace MicroNinja;
using namespace TinySDL;

Game::Game(int res_width, int res_height, int win_width, int win_height, const char * title) {

    window_title = title;
    
    width = res_width;
    height = res_height;

    window_width = win_width;
    window_height = win_height;

    Window::init(title, window_width, window_height, 
                 SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                 SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL, false);

    virtual_projection = LinAlg::ortho(0, (float) width, (float) height, 0, -1, 1);
    window_projection = LinAlg::ortho(0, (float) window_width, (float) window_height, 0, -1, 1); 
}

void Game::run() {   

    renderer.setup();
    
    default_shader = Shader::default_sprite_shaders();
    default_shader.use().set_mat4x4("projection", window_projection); 

    begin();

    auto frame = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<float>{1.0f / target_fps});
    auto next_frame = std::chrono::system_clock::now();
    auto last_frame = next_frame - frame;;

    SDL_Event event; 
    while(!quit_game) {
        while (SDL_PollEvent(&event) != 0) handle_events(event);
        
        update();        
        
        render();

        Window::swap_buffers();
        
        // Frame capping
        std::this_thread::sleep_until(next_frame);
        last_frame = next_frame;
        next_frame += frame;
    }
}

void Game::begin() {
    current_scene.begin();
}

void Game::update() {
    current_scene.update();
}

void Game::render() {
    current_scene.render();
}

void Game::handle_events(SDL_Event & event) {

    if (event.type == SDL_WINDOWEVENT) {
        if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
            Window::get_drawable_size(&window_width, &window_height);
            window_projection = LinAlg::ortho(0, (float) window_width, (float) window_height, 0, -1, 1);                   
        }
    }

    if (event.type == SDL_QUIT) {
        quit_game = true;
    }      
}


// ------------------------------------ Derived class ------------------------------------


void MicroNinjaGame::begin() {

    content.set_folder_name("content");
    content.load_all();

    target = RenderTarget(width, height);

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

        for (int i = 0; i < 10; i++){
            for (int j = 0; j < 10; j++) {
                renderer.draw_tex({16.0f * (float)i, 16.0f * (float)j, 16.0f, 16.0f}, {(float) i * 16.f, (float) j * 16.f});
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




