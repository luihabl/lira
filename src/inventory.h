#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <set>

namespace Lira
{
    class Inventory
    {
    public:
        
        Inventory() = default;

        struct Item
        {
            std::string id = "";
        };

        void add(const std::string& name, const std::string& id)
        {
            if(!contains(name, id))
                items[name].push_back({id});
        }

        size_t count(const std::string& name)
        {
            return items[name].size();
        }

        size_t remove(const std::string& name, size_t amount = 1)
        {
            size_t amount_removed = 0;
            auto& vec = items[name];
            for (int i = 0; i < amount; i++)
            {
                if (vec.size() > 0)
                {
                    vec.pop_back();
                    amount_removed++;
                }
                else
                {
                    break;
                }
            }
            return amount_removed;
        }

        bool contains(const std::string& name, const std::string& id)
        {           
            const auto& vec = items[name];
            for (const auto& item : vec)
            {
                if(item.id == id)
                    return true;
            }

            return false;
        }

        void clear()
        {
            items.clear();
        }

        std::unordered_map<std::string, std::vector<Item>>* get_all()
        {
            return &items;
        }

    private:

        std::unordered_map<std::string, std::vector<Item>> items;

    };
}