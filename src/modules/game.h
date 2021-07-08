#pragma once

#include <string>
#include <chrono>
#include <tinysdl.h>

#include "../core/scene.h"
#include "../assets/content.h"

namespace chr = std::chrono;
using namespace TinySDL;

namespace MicroNinja {

    class Scene;
    
    class Game {
        
        public:
            Game() = default;
            Game(int res_width, int res_height, int win_width, int win_height, const char * title);

            void run();

            std::string window_title;

            int width;
            int height;

            int window_width;
            int window_height;

            Content content;

            BatchRenderer renderer;
            Shader default_shader;
            Mat4x4 virtual_projection;
            Mat4x4 window_projection;

            void move_to_scene(SceneRef & scene);
            void move_to_scene(SceneRef && scene);

            void set_target_fps(float fps);
        
        protected:
            //Called when game starts
            virtual void begin();

            //Called every frame for updating scene
            virtual void update();

            //Called every frame for rendering scene
            virtual void render();

            //Called every frame to handle SDL events
            virtual void handle_events(SDL_Event & event);

        private: 
            void set_current_scene(SceneRef & scene);
            SceneRef current_scene = nullptr;
            SceneRef next_scene = nullptr;

            bool quit_game = false;
            
            float target_fps;
            chr::system_clock::duration frame_duration;
            float dt;
    };

} 
