#pragma once 

#include "../core/ecs.h"
#include "../assets/spritesheet.h"

#include <auk.h>

#include <vector>
#include <unordered_map>
#include <string>

namespace Lira {

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
        void set_frame(SpriteSheet::Frame* frame) { current_frame = frame; 
            //auk::Log::debug("%s [%d]", current_id.c_str(), current_frame_index);
        }

    public:
        AnimatedSprite() = default;
        AnimatedSprite(std::string key);

        void update () override;
        void render(auk::BatchRenderer & renderer) override;

        void connect(const std::string& id, const std::string& next_id);

        void play(const std::string & id, bool restart = false);
        void stop();

        void static_sprite(const Sprite& sprite);

        SpriteSheet::Animation* add(const std::string & id);
        SpriteSheet::Animation* get(const std::string& id);

        auk::Vec2 scale = auk::Vec2::ones;
        bool flip_x = false;
        bool flip_y = false;
    };
}