#pragma once 

#include <vector>

#include <tinysdl.h>
#include "../assets/tileset.h"
#include "../core/component.h"

namespace MicroNinja {

    class TileMap : public Component {

    public:
        TileMap() = default;
        TileMap(int columns, int rows, int tile_w, int tile_h);

        void set_cells(const TileSet & tileset, const std::vector<int> & cx_list,  const std::vector<int> & cy_list, const std::vector<int> & tile_ids);
        void set_cell(int cell_x, int cell_y, const TinySDL::TexRegion & reg);

        void render(TinySDL::BatchRenderer & renderer) override;

    private: 
        
        std::vector<TinySDL::TexRegion> cells;
        int tile_w = 0;
        int tile_h = 0;
        int columns = 0;
        int rows = 0;

    };

}