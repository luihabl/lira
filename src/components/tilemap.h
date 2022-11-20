#pragma once 

#include <vector>

#include <auk.h>
#include "../assets/tileset.h"
#include "../assets/map.h"
#include "../core/ecs.h"

namespace Lira {

    class TileMap : public Component {

    public:
        TileMap() = default;
        TileMap(int columns, int rows, int tile_w, int tile_h);

        bool set_cells(const Map::Layer& layer);
        void set_cell(int cell_x, int cell_y, const auk::TexRegion & reg);

        void render(auk::BatchRenderer & renderer) override;

    private: 
        
        std::vector<auk::TexRegion> cells;
        int tw = 0;
        int th = 0;
        int nx = 0;
        int ny = 0;

    };

}