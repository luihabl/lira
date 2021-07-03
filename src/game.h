#pragma once

#include <string>
#include <tinysdl.h>

#include "scene.h"

using namespace TinySDL;

namespace MicroNinja {
    
    class Game {
        
        public:
            Game() = default;
            Game(int res_width, int res_height, int win_width, int win_height, const char * title);

            void run();

        protected:
            std::string window_title;

            int width;
            int height;

            int window_width;
            int window_height;

            Scene current_scene;

            BatchRenderer renderer;

            //Called when game starts
            virtual void begin();

            //Called every frame for updating scene
            virtual void update();

            //Called every frame for rendering scene
            virtual void render();

            //Called every frame to handle SDL events
            virtual void handle_events(SDL_Event & event);

        private: 
            
            bool quit_game = false;

            float target_fps = 60.0f;

            Shader default_shader;
            Mat4x4 virtual_projection;
            Mat4x4 window_projection;
            

        // Scene current_scene;
        
    };


    class MicroNinjaGame : public Game{
    
        using Game::Game;

        float r;
        
        void begin();
        void render();

    };




} 
