#pragma once 

#include <tinysdl.h>

namespace MicroNinja {

    struct Sprite {
        TinySDL::TexRegion tex;
        TinySDL::IVec2 origin;

        void render(TinySDL::BatchRenderer & renderer, const TinySDL::Vec2 & pos, const TinySDL::Vec2 & scale) {
            renderer.push_transform(TinySDL::LinAlg2D::gen_transform(pos, scale, origin.cast_to<float>(), 0.0f));
            renderer.draw_tex(tex);
            renderer.pop_transform();
        }

    };
}