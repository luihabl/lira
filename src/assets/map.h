#pragma once


#include "tileset.h"

#include <auk.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <string>
#include <memory>

//Don't expose json.hpp out of this class
namespace Lira
{
    class Map
    {
    
    public:

        Map() = default;
        Map(const nlohmann::json& json, const std::string& key);

        enum class TileFlag // Change this to bit flag where you can combine more than one flag wit or-ing.
        {
            NONE,
            SOLID,
            ONE_WAY,
            DANGER
        };

        struct Layer
        {
            TileSet* tileset = nullptr; 
            struct Tile
            {
                int tile = 0;
                int x = 0, y = 0;
                bool fx = false, fy = false;

                TileFlag flag = TileFlag::NONE; //optional
            };

            std::vector<Tile> tiles;

            int dx, dy;
            int columns, rows;
        };

        struct Object
        {
            auk::IVec2 pos;
            std::string name;
            std::string unique_id;
            //extend for other properties
        };

        struct Room
        {
            auk::IntRect bbox;
            std::vector<Layer> layers;
            std::vector<Object> objects;
        };

        std::vector<std::shared_ptr<TileSet>> tilesets;
        std::vector<Room> rooms;
    };
}
