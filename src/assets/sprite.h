#pragma once 

#include <tinysdl.h>

namespace Lira {

    struct Sprite {
        TinySDL::TexRegion tex;
        TinySDL::IVec2 origin;
        
        void render(TinySDL::BatchRenderer & renderer, const TinySDL::Vec2 & pos, bool flip_x, bool flip_y, TinySDL::Vec2 scale) {

            if(flip_x)
                scale[0] = -scale[0];
            
            if(flip_y)
                scale[1] = -scale[1];

            renderer.push_transform(TinySDL::LinAlg2D::gen_transform(pos, scale, origin.cast_to<float>(), 0.0f));
            renderer.draw_tex(tex);
            renderer.pop_transform();
        }

    };
}