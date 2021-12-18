#pragma once 

#include "input.h"
#include "tinysdl.h"

namespace Lira {

    class VirtualInput {

    public:
        virtual void update() = 0; 
        virtual VirtualInput& operator=(const VirtualInput& other)
        {
            registered = registered || other.registered;
            return *this;
        }

        virtual VirtualInput& VirtualInput::register_input() 
        {

            if (!registered) {
                registered = true;
                Input::register_button(this);
            }

            return *this;
        }

        virtual ~VirtualInput() 
        {
            if (registered)
            {
                Input::unregister_button(this);
            }
        }
            
    protected:
        bool registered = false;
        
    };
}