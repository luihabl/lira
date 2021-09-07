#include <tinysdl.h>

namespace MicroNinja
{
    
    template <typename T>
    class TypeTable
    {
    public:
        TypeTable() = default;
        ~TypeTable()
        {
            for (auto [id, items] : table)
                for (auto item : items)
                    delete item;
        }

        template <typename G>
        void add(G* c)
        {
            table[TinySDL::Type::type_of<G>()].push_back((T*) c);
        }
        
        void erase(T* c)
        {
            auto& vec = table[c->type]; // <---- maybe change to TinySDL::Type::type_of<G>()?

            for (size_t i = 0; i < vec.size(); i++)
            {
                if (vec[i] == (T*) c)
                {
                    vec[i] = vec.back();
                    vec.pop_back();
                    delete c;
                    return;
                }
            }
        }

        template <typename G>
        const std::vector<T*>& get_group()
        {
            return table[TinySDL::Type::type_of<G>()];
        }

        auto begin()
        {
            return table.begin();
        }

        auto end()
        {
            return table.end();
        }

    private:
        std::unordered_map<size_t, std::vector<T*>> table;

    };
}