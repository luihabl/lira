#include "virtualaxis.h"

#include "keys.h"
#include "input.h"


using namespace Lira;

VirtualAxis::~VirtualAxis(){
    if(registered) Input::unregister_button(this);
}

VirtualAxis & VirtualAxis::register_input() {

    if(!registered) {
        registered = true;
        Input::register_button(this);
    }

    return *this;
}

VirtualAxis & VirtualAxis::add(Key negative, Key positive) {
    nodes.push_back({negative, positive});
    return *this;
}

void VirtualAxis::update() {
    
    for(auto & node : nodes)
        node.update();

    val = 0;
    for(auto & node : nodes) {
        if(node.val != 0) {
            val = node.val;
            break;
        }
    }
}

void VirtualAxis::KeyAxisNode::update() {

    if (Input::pressed(positive)) {

        if (Input::pressed(negative)) {
            val = 0.0f;
        }
        else {
            val = 1.0f;
        }
    }
    else if (Input::pressed(negative)) {
        val = -1.0f;
    }
    else {
        val = 0.0f;
    }

}