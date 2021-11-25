#pragma once 

#include <functional>
#include <unordered_map>

namespace MicroNinja
{
    template<typename T>
    class StateMachine 
    {
        size_t last = 0;
        size_t state = 0;

        using FType = std::function<void(T*, StateMachine*)>;

        struct State
        {
            FType begin = nullptr;
            FType update = nullptr;
            FType end = nullptr;
        };

        std::unordered_map<size_t, State> states;
        
    public:

        StateMachine() = default;

        void add(size_t id, FType begin = nullptr, FType update = nullptr, FType end = nullptr)
        {
            states[id] = { begin, update, end };
        }

        void set(size_t id, T* e)
        {
            if (id != state)
            {
                if (states[id].end)
                    states[id].end(e, this);

                last = state;
                state = id;

                if (states[id].begin)
                    states[id].begin(e, this);
            }
        }

        void update(T* e)
        {
            if (states[id].update)
                states[id].update(e, this);
        }

        size_t get_state() { return state; }
        size_t get_last() { return last; }
        
    };
}
