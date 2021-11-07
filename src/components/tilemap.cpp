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

bool TileMap::set_cells(const Map::Layer& layer) {

    bool has_flag = false;
    
    const auto& tileset = *layer.tileset;

    for(const auto& tile: layer.tiles)
    {
        cells[tile.x + tile.y * nx] = tileset[tile.tile];

        if(tile.fy)
            cells[tile.x + tile.y * nx].flip_y();
            
        if(tile.fx)
            cells[tile.x + tile.y * nx].flip_x();
        


        if(tile.flag != Map::TileFlag::NONE)
            has_flag = true;
    }

    return has_flag;
}

