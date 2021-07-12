#include <tinysdl.h>
#include "tileset.h"

using namespace MicroNinja;
using namespace TinySDL;

TileSet::TileSet(int tile_w, int tile_h, Texture * tex) {
    
    ASSERT_MSG(tex, "Empty texture");

    tile_w = tile_w;
    tile_h = tile_h;

    columns = tex->w / tile_w;
    rows = tex->h / tile_h;

    tiles.resize( columns * rows );

    for (int cy = 0; cy < rows; cy++)
        for (int cx = 0; cx < columns; cx++) {
            tiles[cx + cy * columns] = TexRegion(tex, {(float) cx * tile_w, (float) cy * tile_h, (float) tile_w, (float) tile_h});
        }



}