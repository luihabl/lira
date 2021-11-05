#pragma once


#include "tileset.h"

#include <tinysdl.h>
#include <nlohmann/json.hpp>

#include <vector>
#include <string>

//Don't expose json.hpp out of this class
namespace MicroNinja
{
    class Map
    {
    
    public:

        Map() = default;
        Map(const nlohmann::json& json, const std::string& key);

        enum CollisionType
        {
            NONE,
            SOLID,
            ONE_WAY,
            DANGER
        };

        struct Layer
        {
            TileSet* tileset; 
            struct Tile
            {
                int tile;
                int x, y;
                bool fx, fy;

                CollisionType coll = NONE; //optional
            };

            std::vector<Tile> tiles;
        };

        struct Object
        {
            TinySDL::IVec2 pos;
            std::string name;
            //extend for other properties
        };

        struct Room
        {
            TinySDL::IntRect bbox;
            std::vector<Layer> layers;
            std::vector<Object> objects;

            int columns;
            int rows;
        };

        std::vector<TileSet> tilesets;
        std::vector<Room> rooms;
    };
}
