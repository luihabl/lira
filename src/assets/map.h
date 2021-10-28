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
            SOLID,
            ONE_WAY,
            DANGER
        };

        struct Layer
        {
            TileSet* tileset; 
            std::vector<int> tiles; 
            std::vector<int> x, y, flip; 

            //Optional
            std::vector<CollisionType> coll;
        };

        struct Object
        {
            TinySDL::IVec2 pos;
            std::string name;
            //extend for other properties
        };

        struct Room
        {
            // TinySDL::IVec2 pos;
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
