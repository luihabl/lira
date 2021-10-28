#include "map.h"
#include "content.h"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

#include <iostream>


using namespace MicroNinja;
using namespace TinySDL;

Map::Map(const nlohmann::json& json, const std::string& key)
{

    std::unordered_map<int, int> tileset_uid;
    
    // Load TileSets
    int ti = 0;
    for(const auto& tileset : json["defs"]["tilesets"])
    {
        
        std::filesystem::path tex_path = key;
        tex_path = tex_path.remove_filename();
        tex_path /= tileset["relPath"].get<std::string>();
        tex_path = Content::normalize_key((tex_path.parent_path() / tex_path.stem()).generic_string());

        tilesets.push_back(TileSet(
            tileset["tileGridSize"].get<int>(), 
            tileset["tileGridSize"].get<int>(), 
            Content::find<Texture>(tex_path.generic_string())
            ));

        tileset_uid[tileset["uid"].get<int>()] = ti++;
    }

    // Load Rooms
    for(const auto& level : json["levels"])
    {
        Room room;
        room.bbox = IntRect(level["worldX"].get<int>(), level["worldY"].get<int>(), level["pxHei"].get<int>(), level["pxWid"].get<int>());

        for(const auto& layer : level["layerInstances"])
        {
            std::string layer_type = layer["__type"].get<std::string>();
            
            if(layer_type == "Tiles")
            {

            }
            else if(layer_type == "Entities")
            {

            }
            else if(layer_type == "IntGrid")
            {

            }
            else if(layer_type == "AutoLayer")
            {

            }



        }





        // rooms.push_back();
    }


}