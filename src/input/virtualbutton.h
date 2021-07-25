#pragma once 

#include "keys.h"
#include <tinysdl.h>
#include <vector>

namespace MicroNinja {

    class VirtualButton {
        
        public:
            VirtualButton() = default;
            VirtualButton & add(Key key);
            VirtualButton & register_input();
            VirtualButton & set_repeat(float first_delay, float multi_delay);
            ~VirtualButton();

            void update();

            bool pressed(){return btn_pressed;}
            bool just_pressed(){return btn_just_pressed;}
            bool released(){return btn_released;}

            bool can_repeat = false;

        private:

            struct KeyNode {

                KeyNode(Key k) : key(k) {};
                Key key;
                bool check();
            };

            std::vector <KeyNode> nodes;

            bool repeating = false;

            TinySDL::SimpleTimer repeat_timer;
            bool first_repeat = true;
            float repeat_first_delay_ms = 0.0f;
            float repeat_multi_delay_ms = 0.0f; 

            bool btn_pressed = false;
            bool btn_just_pressed = false;
            bool btn_released = false;

            bool registered = false;
    
    };
}