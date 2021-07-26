#pragma once 

#include "virtualinput.h"
#include "keys.h"
#include <tinysdl.h>
#include <vector>

namespace MicroNinja {

    class VirtualAxis : public VirtualInput  {
        
        public:
            VirtualAxis() = default;
            VirtualAxis & add(Key negative, Key positive);
            VirtualAxis & register_input();
            ~VirtualAxis();

            void update() override;

            float value() { return val; }

        private:

            struct KeyAxisNode {

                KeyAxisNode(Key n, Key p) : negative(n), positive(p) {};
                Key negative, positive;

                float val = 0.0f;

                void update();
            };

            std::vector <KeyAxisNode> nodes;

            float val = 0.0f;
            bool registered = false;
    
    };
}