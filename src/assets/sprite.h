#pragma once 

#include <auk.h>

namespace Lira {

    struct Sprite {
        auk::TexRegion tex;
        auk::IVec2 origin;
        
        void render(auk::BatchRenderer & renderer, const auk::Vec2 & pos, bool flip_x, bool flip_y, auk::Vec2 scale) {

            if(flip_x)
                scale[0] = -scale[0];
            
            if(flip_y)
                scale[1] = -scale[1];

            renderer.push_transform(auk::LinAlg2D::gen_transform(pos, scale, origin.cast_to<float>(), 0.0f));
            renderer.draw_tex(tex);
            renderer.pop_transform();
        }

    };
}