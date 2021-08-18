#include "collider_grid.h"

#include <tinysdl.h>

using namespace MicroNinja;
using namespace TinySDL;

ColliderGrid::ColliderGrid(int columns, int rows, int cell_w, int cell_h) {
    grid = VirtualMap<bool>(columns, rows, cell_w, cell_h);
}

void ColliderGrid::render(BatchRenderer & renderer) {

    for(int y = 0; y < grid.ny; y++) {
        for(int x = 0; x < grid.nx; x++) {
            if (grid.cells[ y * grid.nx + x ]) 
                renderer.draw_rect_line({(float) x * grid.w, (float) y * grid.h, (float) grid.w, (float) grid.h}, Color::red, 1);
        }
    }
}
