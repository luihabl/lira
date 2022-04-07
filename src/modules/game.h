#pragma once

#include <string>
#include <chrono>
#include <tinysdl.h>

#include "../input/input.h"
#include "../core/ecs.h"
#include "../assets/content.h"

namespace chr = std::chrono;

namespace Lira {

    class Scene;
    
    class Game {
        
        public:
            Game() = default;
            Game(int res_width, int res_height, int win_width, int win_height, const char * title);

            void run();
            void quit();

            std::string window_title;

            int width;
            int height;

            int window_width;
            int window_height;

            Content content;

            TinySDL::BatchRenderer renderer;
            TinySDL::Shader default_shader;
            TinySDL::Mat4x4 virtual_projection;
            TinySDL::Mat4x4 window_projection;

            // Receives a unique pointer to a scene
            void move_to_scene(SceneRef&& scene);
            void move_to_scene(SceneRef & scene);
            

            void set_target_fps(float fps);
        
        protected:
            //Called when game starts
            virtual void begin();

            //Called every frame for updating scene
            virtual void update();

            //Called every frame for rendering scene
            virtual void render();

            //Called when game ends
            virtual void end();

            //Called every frame to handle SDL events
            InputHandler input_handler;

            void set_window_projection(int window_w, int window_h);

        private: 
            void set_current_scene(SceneRef & scene);
            SceneRef current_scene = nullptr;
            SceneRef next_scene = nullptr;

            bool quit_game = false;

            chr::system_clock::duration frame_duration;
    };

    namespace GameProperties {
        float delta_time();
        float fps();
    }

} 
