#pragma once

#include <vector>
#include <unordered_map>
#include <string>

namespace Lira
{
    class Inventory
    {
    public:
        
        Inventory() = default;

        struct Item
        {
            std::string name = "";
            std::string id = "";
            int amount = 0;
        };

        void add(const std::string& name, const std::string& id);
        void remove(const std::string& id);
        void clear();

    private:

        std::unordered_map<std::string, Item> items;

    };
}