#include <tinysdl.h>
#include "tileset.h"

using namespace Lira;
using namespace TinySDL;

TileSet::TileSet(int _tile_w, int _tile_h, Texture * _tex) {
    
    ASSERT_MSG(_tex, "Empty texture");

    tex = _tex;

    tile_w = _tile_w;
    tile_h = _tile_h;

    columns = tex->w % tile_w == 0 ? tex->w / tile_w : tex->w / tile_w + 1;
    rows = tex->h % tile_h == 0 ? tex->h / tile_h : tex->h / tile_h + 1;

    tiles.resize( columns * rows );

    for (int cy = 0; cy < rows; cy++)
        for (int cx = 0; cx < columns; cx++) {
            tiles[cx + cy * columns] = TexRegion(tex, {(float) cx * tile_w, (float) cy * tile_h, (float) tile_w, (float) tile_h});
        }



}