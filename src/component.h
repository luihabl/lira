#pragma once

namespace MicroNinja {

    class Component {

        virtual void begin() {};
        virtual void update() {};
        virtual void render() {};

    };
}