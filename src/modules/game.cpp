#include <tinysdl.h>

#include <chrono>
#include <thread>
#include <utility>

#include "../input/input.h"
#include "game.h"

namespace chr = std::chrono;
using namespace Lira;
using namespace TinySDL; 

constexpr float default_fps = 60.0f;

namespace {
    float dt;
    float target_fps;
}

float GameProperties::delta_time() {
    return dt;
}

float GameProperties::fps() {
    return target_fps;
}

Game::Game(int res_width, int res_height, int win_width, int win_height, const char * title) {

    window_title = title;
    
    width = res_width;
    height = res_height;

    window_width = win_width;
    window_height = win_height;

    Window::init(title, window_width, window_height);

    virtual_projection = LinAlg::ortho(0, (float) width, (float) height, 0, -1, 1);
    window_projection = LinAlg::ortho(0, (float) window_width, (float) window_height, 0, -1, 1); 

    set_target_fps(default_fps);

    input_handler.on_quit = [this](){quit();};
    input_handler.on_window_resize = [this](int w, int h){set_window_projection(w, h);};
}

void Game::run() {   

    renderer.setup();
    
    default_shader = Shader::default_sprite_shaders();
    default_shader.use().set_mat4x4("projection", window_projection); 

    begin();

    auto next_frame = chr::system_clock::now();
    auto last_frame = next_frame - frame_duration;

    while(!quit_game) {

        Input::update(input_handler);

        // Changing scenes
        if(next_scene != nullptr && next_scene != current_scene) {
            current_scene.reset();
            set_current_scene(next_scene);
            current_scene->begin();
        }
        
        // Updating scene
        update();        
        
        // Rendering scene
        render();

        Window::swap_buffers();
        
        // Frame capping
        std::this_thread::sleep_until(next_frame);
        last_frame = next_frame;
        next_frame += frame_duration;
    }
}

void Game::begin() {
    if(current_scene) current_scene->begin();
}

void Game::update() {
    if(current_scene) current_scene->update();
}

void Game::render() {
    if(current_scene) current_scene->render(renderer);
}

void Game::quit() {
    quit_game = true;
}

void Game::set_window_projection(int window_w, int window_h) {
    window_width = window_w;
    window_height = window_h;
    window_projection = LinAlg::ortho(0, (float) window_width, (float) window_height, 0, -1, 1);
}

void Game::move_to_scene(SceneRef & scene) {
    next_scene = std::move(scene);
}

void Game::move_to_scene(SceneRef && scene) {
    next_scene = std::move(scene);
}

void Game::set_current_scene(SceneRef & scene) {
   current_scene = std::move(scene);
   current_scene->game = this;
}

void Game::set_target_fps(float fps) {
    target_fps = fps;
    dt = 1.0f/target_fps;
    frame_duration = chr::duration_cast<chr::system_clock::duration>(chr::duration<float>{dt});
}
