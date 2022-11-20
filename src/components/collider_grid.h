#pragma once 

#include "../core/ecs.h"
#include "../util/virtual_map.h"

#include <auk.h>
#include <array>

namespace Lira {

    class ColliderGrid : public Component {
    
    public:
        ColliderGrid() = default;
        ColliderGrid(int columns, int rows, int cell_w, int cell_h);

        void set_cell(int x, int y, bool val){ grid.cells[ y * grid.nx + x ] = val; }
        
        bool has_cell(int x, int y ){ 
            if (x < 0 || x >= grid.nx || y < 0 || y >= grid.ny)
                return false;
        
            return grid.cells[ y * grid.nx + x ]; 
        }
        
        std::array<int, 2> get_cell_size() {return {grid.w, grid.h}; }

        void render(auk::BatchRenderer & renderer) override;

        uint32_t layer = -1;

    private:
        
        VirtualMap<bool> grid;

        // auk::IntRect rect;
        // auk::IntRect scene_rect();
    };
}