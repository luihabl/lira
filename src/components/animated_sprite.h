#pragma once 

#include "../core/component.h"
#include "../assets/sprite.h"

#include <tinysdl.h>

#include <vector>
#include <unordered_map>
#include <string>

namespace MicroNinja {

    class AnimatedSprite : public Component {
    
    private:

        struct Frame : Sprite {
            float delay_ms;
        };

        struct Animation {
            size_t size(){ return frames.size(); }
            std::vector<Frame> frames;
            bool loop = true;
        };

        std::string current_id = "";
        std::unordered_map<std::string, Animation> animations;
        
        Animation * current_animation = nullptr;
        Frame * current_frame = nullptr;

        float animation_timer = 0.0f;
        size_t current_frame_index;
        bool playing = false;

        size_t animation_lenght(){ return current_animation->size(); }
        void set_frame(Frame * frame) { current_frame = frame; }

    public:
        void update () override;
        void render(TinySDL::BatchRenderer & renderer) override;

        void play(const std::string & id, bool restart = false);
        void stop();

        Animation * add(const std::string & id);

        TinySDL::Vec2 scale = TinySDL::Vec2::ones;
        // bool flip_x = false;
        // bool flip_y = false;
    };
}