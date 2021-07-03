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

    
    Game::begin();

}

void MicroNinjaGame::render() {

    Graphics::viewport(window_width, window_height);
    Graphics::clear({5, 33, 77});

    renderer.draw_circle_fill({450, 200}, 50, Color::black);
    renderer.draw_rect_fill({40, 40, 50, 50}, Color::blue);

    renderer.render();

    Game::render();

}




