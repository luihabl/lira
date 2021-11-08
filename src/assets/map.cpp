#include "map.h"
#include "content.h"

#include <nlohmann/json.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

#include <iostream>
#include <algorithm>

using namespace MicroNinja;
using namespace TinySDL;

Map::Map(const nlohmann::json& json, const std::string& key)
{

    std::unordered_map<int, int> tileset_uid;
    std::unordered_map<int, std::unordered_map<std::string, std::vector<int>>> tilset_tags;

    // Load TileSets
    int ti = 0;
    for(const auto& tileset : json["defs"]["tilesets"])
    {
        
        std::filesystem::path tex_path = key;
        tex_path = tex_path.remove_filename();
        tex_path /= tileset["relPath"].get<std::string>();
        tex_path = Content::normalize_key((tex_path.parent_path() / tex_path.stem()).generic_string());

        tilesets.push_back(std::make_shared<TileSet>(
            tileset["tileGridSize"].get<int>(), 
            tileset["tileGridSize"].get<int>(), 
            Content::find<Texture>(tex_path.generic_string())
            ));

        int tuid = tileset["uid"].get<int>();
        tileset_uid[tuid] = ti++;
    
        for(const auto& tag : tileset["enumTags"])
            tilset_tags[tuid][tag["enumValueId"]] = tag["tileIds"].get<std::vector<int>>();
    }

    // Load Rooms
    for(const auto& level : json["levels"])
    {
        Room room;
        room.bbox = IntRect(level["worldX"].get<int>(), level["worldY"].get<int>(), level["pxWid"].get<int>(), level["pxHei"].get<int>());
    
        for(const auto& layer : level["layerInstances"])
        {
            std::string layer_type = layer["__type"].get<std::string>();
            
             // Layer types
            if(layer_type == "Tiles" || layer_type == "IntGrid")
            {
                Layer l;

                int tuid = layer["__tilesetDefUid"].get<int>();
                l.tileset = tilesets[tileset_uid[tuid]].get();

                l.dx = layer["__gridSize"];
                l.dy = layer["__gridSize"];

                l.columns = (room.bbox.w % l.dx == 0 ? room.bbox.w / l.dx : room.bbox.w / l.dx + 1);
                l.rows = (room.bbox.h % l.dy == 0 ? room.bbox.h / l.dy : room.bbox.h / l.dy + 1);
                
                const auto& tags = tilset_tags[tuid];

                nlohmann::json tiles;
                if(layer_type == "Tiles")
                    tiles = layer["gridTiles"];
                if(layer_type == "IntGrid")
                    tiles = layer["autoLayerTiles"];

                for(const auto& tile : tiles)
                {
                    Layer::Tile t;
                    
                    t.tile = tile["t"].get<int>();

                    t.x = tile["px"][0] / l.dx;
                    t.y = tile["px"][1] / l.dy;

                    const auto flip = tile["f"].get<int>();
                    t.fx = flip & 0x1;
                    t.fy = flip & 0x2;

                    for(const auto& tag : tags)
                    {
                        if(tag.second.size() == 0)
                            continue;
                        
                        // Make flag assignment automatic and more efficient
                        if(std::find(tag.second.begin(), tag.second.end(), t.tile) != tag.second.end())
                        {
                            if(tag.first == "Solid")
                            {
                                t.flag = TileFlag::SOLID;
                            }
                            else if (tag.first == "OneWaySolid")
                            {
                                t.flag = TileFlag::ONE_WAY;
                            }
                            else if (tag.first == "Damage")
                            {
                                t.flag = TileFlag::DANGER;
                            }
                        }
                    }

                    l.tiles.push_back(t);
                }

                room.layers.push_back(l);
            }
            else if(layer_type == "AutoLayer")
            {

            }
            else if(layer_type == "Entities")
            {
                for(const auto& object : layer["entityInstances"])
                {                    
                    Object o;
                    o.name = object["__identifier"];
                    o.pos = IVec2({object["px"][0], object["px"][1]});

                    room.objects.push_back(o);
                }

            }
        }

        std::reverse(room.layers.begin(), room.layers.end()); // this is needed because ldtk use reversed ordered layers.
        rooms.push_back(room);
    }
}