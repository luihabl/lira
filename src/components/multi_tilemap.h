#pragma once

#include "../core/ecs.h"
#include "tilemap.h"

#include <tinysdl.h>

#include <vector>

namespace Lira
{
    class MultiTileMap : public Component
    {

    public:
        MultiTileMap() = default;

        void render(TinySDL::BatchRenderer &renderer) override
        {
            for (TileMap &tilemap : tilemaps)
                tilemap.render(renderer);
        }

        void set_tilemaps(const std::vector<TileMap> &maps) 
        { 
            tilemaps = maps; 
            for (auto& t : tilemaps)
                t.entity = entity;
        }

    private:
        std::vector<TileMap> tilemaps;
    };
}