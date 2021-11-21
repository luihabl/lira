#pragma once 

#include <functional>
#include <array>

namespace MicroNinja
{
    template<typename T, size_t NStates>
    class StateMachine 
    {
        size_t state = 0;

        using FType = std::function<void(T*)>;

        std::array<FType, NStates> begins;
        std::array<FType, NStates> updates;
        std::array<FType, NStates> ends; 

    public:

        StateMachine() = default;

        void add(size_t id, FType begin = nullptr, FType update = nullptr, FType end = nullptr)
        {
            begins[id] = begin;
            updates[id] = update;
            ends[id] = end;
        }

        void set(size_t id, T* e)
        {
            if (id != state)
            {
                if (ends[id])
                    ends[id](e);

                state = id;
                if (begins[state])
                    begins[state](e);
            }
        }

        void update(T* e)
        {
            if (updates[state])
                updates[state](e);
        }
        
    };
}
