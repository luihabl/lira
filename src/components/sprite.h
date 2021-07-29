#pragma once 

#include "../core/component.h"

#include <tinysdl.h>

#include <vector>
#include <unordered_map>
#include <string>

namespace MicroNinja {


    class AnimatedSprite : public Component {
    
    public:
        void update () override;
        void render(TinySDL::BatchRenderer & renderer) override;

        void play(const std::string & id, bool restart = false);
        void stop();

    private:

        struct Frame {
            TinySDL::TexRegion tex;
            float delay;
        };

        struct Animation {
            size_t size(){ return frames.size(); }
            std::vector<Frame> frames;
            bool loop;
        };

        std::string current_id;
        std::unordered_map<std::string, Animation> animations;
        Animation * current_animation;
        Frame * current_frame;

        float animation_timer;
        size_t current_frame_index;
        bool playing;


        size_t animation_lenght(){ return current_animation->size(); }
        void set_frame(Frame * frame) { current_frame = frame; }


  
    };
}