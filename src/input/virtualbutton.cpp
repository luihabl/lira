#include "virtualbutton.h"
#include "keys.h"
#include "input.h"

using namespace MicroNinja;


VirtualButton & VirtualButton::add(Key key) {
    nodes.push_back(key);
    return *this;
}

void VirtualButton::update() {

    bool last_press = _pressed;
    _pressed = false;
    for(auto & node : nodes) {
        if(node.check()) {
            _pressed = true;
            break;
        }
    }

    _just_pressed = false;
    if ((_pressed && !last_press) || repeating) {
        _just_pressed = true;
    }
    
    _released = false;
    if (last_press && !_pressed) {
        _released = true;
    }
    
}

bool VirtualButton::KeyNode::check() {
    return Input::pressed(key);
}