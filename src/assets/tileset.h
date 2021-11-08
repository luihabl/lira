#pragma once 

#include <tinysdl.h>
#include <vector>

namespace MicroNinja {

    class TileSet {
    
    public:
        TileSet() = default;
        TileSet(int tile_w, int tile_h, TinySDL::Texture * tex);

        TinySDL::TexRegion operator [](size_t i) const {return tiles[i];}
        
        int dx() const { return tile_w; }
        int dy() const { return tile_h; }
        int c() const { return columns; }
        int r() const { return rows; }

    private:
        TinySDL::Texture * tex;
        std::vector<TinySDL::TexRegion> tiles;

        int tile_w = 0;
        int tile_h = 0;
        int columns = 0;
        int rows = 0;

    };
}