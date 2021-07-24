#pragma once 

#include "keys.h"

#include <vector>

namespace MicroNinja {

    class VirtualButton {
        
        public:
            VirtualButton() = default;
            VirtualButton & add(Key key);

            void update();

            bool pressed(){return _pressed;}
            bool just_pressed(){return _just_pressed;}
            bool released(){return _released;}


            struct Group {

                Group() = default;
                Group(const std::vector<VirtualButton *> & b) : buttons(b){}
                std::vector<VirtualButton *> buttons;
                void update(){for(auto & b : buttons) b->update();}
            };
        
        
        private:

            struct KeyNode {

                KeyNode(Key k) : key(k) {};
                Key key;
                bool check();
            };

            std::vector <KeyNode> nodes;

            bool repeating = false;
            bool _pressed = false;
            bool _just_pressed = false;
            bool _released = false;
    };
}