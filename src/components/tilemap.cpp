#include <algorithm>
#include <tinysdl.h>
#include "tilemap.h"

using namespace MicroNinja;
using namespace TinySDL;

TileMap::TileMap(int columns, int rows, int tile_w, int tile_h) {
    columns = columns;
    rows = rows;
    tile_w = tile_w;
    tile_h = tile_h;

    cells.resize(columns * rows);
}

void TileMap::set_cell(int cell_x, int cell_y, const TinySDL::TexRegion & reg) {
    cells[cell_x + cell_y * columns] = reg;
}

void TileMap::render(TinySDL::BatchRenderer & renderer) {
    for (int cy = 0; cy < rows; cy++)
        for (int cx = 0; cx < columns; cx++) {
            renderer.draw_tex(cells[cx + cy * columns], {(float) cx * tile_w, (float) cy * tile_h});
        }
}

void TileMap::set_cells(const TileSet & tileset, const std::vector<int> & cx_list,  const std::vector<int> & cy_list, const std::vector<int> & tile_ids) {
    for(int i = 0; i < std::min(cx_list.size(), cy_list.size(), tile_ids.size()); i++) 
        cells[cx_list[i] + cy_list[i] * columns] = tileset[tile_ids[i]];
}

