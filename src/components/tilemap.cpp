#include "tilemap.h"

#include "../core/entity.h"
#include <tinysdl.h>

#include <algorithm>


using namespace MicroNinja;
using namespace TinySDL;

TileMap::TileMap(int columns, int rows, int tile_w, int tile_h) {
    nx = columns;
    ny = rows;
    tw = tile_w;
    th = tile_h;

    cells.resize(columns * rows);
}

void TileMap::set_cell(int cell_x, int cell_y, const TinySDL::TexRegion & reg) {
    cells[cell_x + cell_y * nx] = reg;
}

void TileMap::render(TinySDL::BatchRenderer & renderer) {

    renderer.push_transform(LinAlg2D::gen_translation((float)entity->position[0], (float)entity->position[1]));

    for (int cy = 0; cy < ny; cy++)
        for (int cx = 0; cx < nx; cx++) {
            renderer.draw_tex(cells[cx + cy * nx], {(float) (cx * tw), (float) (cy * th)});
        }
    
    renderer.pop_transform();
}

void TileMap::set_cells(const TileSet & tileset, const std::vector<int> & cx_list,  const std::vector<int> & cy_list, const std::vector<int> & tile_ids, const std::vector<int> & flip) {
    for(int i = 0; i <  cx_list.size(); i++) 
    {
        cells[cx_list[i] + cy_list[i] * nx] = tileset[tile_ids[i]];

        if(flip[i] & 0x1)
        {
            cells[cx_list[i] + cy_list[i] * nx].flip_x();
        }

        if(flip[i] & 0x3)
        {
            cells[cx_list[i] + cy_list[i] * nx].flip_y();
        }

    }
        
}

