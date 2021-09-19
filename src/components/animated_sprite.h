#pragma once 

#include "../core/component.h"
#include "../assets/spritesheet.h"

#include <tinysdl.h>

#include <vector>
#include <unordered_map>
#include <string>

namespace MicroNinja {

    class AnimatedSprite : public Component {
    
    private:
        std::string current_id = "";
        std::unordered_map<std::string, SpriteSheet::Animation> animations;
        
        SpriteSheet::Animation * current_animation = nullptr;
        SpriteSheet::Frame * current_frame = nullptr;

        float animation_timer = 0.0f;
        size_t current_frame_index = 0;
        bool playing = false;

        size_t animation_lenght(){ return current_animation->size(); }
        void set_frame(SpriteSheet::Frame * frame) { current_frame = frame; }

    public:
        AnimatedSprite() = default;
        AnimatedSprite(std::string key);

        void update () override;
        void render(TinySDL::BatchRenderer & renderer) override;

        void play(const std::string & id, bool restart = false);
        void stop();

        SpriteSheet::Animation* add(const std::string & id);
        SpriteSheet::Animation* get(const std::string& id);

        TinySDL::Vec2 scale = TinySDL::Vec2::ones;
        bool flip_x = false;
        bool flip_y = false;
    };
}