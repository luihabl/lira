#pragma once

#include <string>
#include <tinysdl.h>

using namespace TinySDL;

namespace MicroNinja {
    
    class Game {
        
        public:
            Game() = default;
            Game(int res_width, int res_height, int win_width, int win_height, const char * title);
            // ~Game();

            void run();

        protected:
            std::string window_title;

            int width;
            int height;

            int window_width;
            int window_height;

            virtual void update() {}
            virtual void render() {}

        private: 
            
            float target_fps = 60.0f;



            // BatchRenderer renderer;
            

        // Scene current_scene;
        
    };


    class MicroNinjaGame : public Game{
    
        using Game::Game;
        
        void update();




    };




} 
