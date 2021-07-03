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
}

void Game::run() {   
    
    // Mat4x4 window_projection = LinAlg::ortho(0, (float) window_width, (float) window_height, 0, -1, 1);
    
    // BatchRenderer renderer;

    // Shader shader = Shader::default_sprite_shaders();
    // shader.use().set_mat4x4("projection", window_projection); 

    // Texture sprite_tex = Texture::from_file("sprite.png");


    auto frame = std::chrono::duration_cast<std::chrono::system_clock::duration>(std::chrono::duration<float>{1.0f / target_fps});
    auto next_frame = std::chrono::system_clock::now();
    auto last_frame = next_frame - frame;;

    SDL_Event event; 
    bool quit = false;
    while(!quit) {
        while (SDL_PollEvent(&event) != 0) 
            if (event.type == SDL_QUIT) 
                quit = true;


        update();
        
        render();


        
        // Graphics::clear(Color::black);

        // renderer.set_texture(&sprite_tex);
        // renderer.draw_tex({50.0f, 150.0f});

        // renderer.render();
        Window::swap_buffers();

        
        // Frame capping
        std::this_thread::sleep_until(next_frame);
        last_frame = next_frame;
        next_frame += frame;
    }
}





// ------------------------------------ Derived class ------------------------------------

void MicroNinjaGame::update() {
    Graphics::viewport(window_width, window_height);
    Graphics::clear({5, 33, 77});
}