#pragma once 

#include <auk.h>
#include <vector>

namespace Lira {

    class TileSet {
    
    public:
        TileSet() = default;
        TileSet(int tile_w, int tile_h, auk::Texture * tex);

        auk::TexRegion operator [](size_t i) const {return tiles[i];}
        
        int dx() const { return tile_w; }
        int dy() const { return tile_h; }
        int c() const { return columns; }
        int r() const { return rows; }

    private:
        auk::Texture * tex;
        std::vector<auk::TexRegion> tiles;

        int tile_w = 0;
        int tile_h = 0;
        int columns = 0;
        int rows = 0;

    };
}