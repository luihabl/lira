#include "animated_sprite.h"
#include <tinysdl.h>

#include "../modules/game.h"
#include "../assets/content.h"

#include <string>

using namespace MicroNinja;
using namespace TinySDL;

AnimatedSprite::AnimatedSprite(std::string key)
{
    animations = Content::find<SpriteSheet>(key)->animations;
}


SpriteSheet::Animation * AnimatedSprite::add(const std::string & id) {

    if ( animations.find(id) == animations.end() ) {
        animations[id] = SpriteSheet::Animation();
    } 

    return &(animations[id]);
}

SpriteSheet::Animation* MicroNinja::AnimatedSprite::get(const std::string& id)
{
    return &(animations[id]);
}


void AnimatedSprite::update() {

    if(playing) {

        animation_timer += 1000.0f * GameProperties::delta_time();
        if(animation_timer >= current_frame->delay_ms) {
            
            animation_timer = 0.0f;
            current_frame_index += 1;

            if (current_frame_index >= animation_lenght()) {
                
                if (current_animation->next_id != "")
                {
                    play(current_animation->next_id);
                }
                else if (current_animation->loop) {
                    current_frame_index = 0;
                    set_frame(&(current_animation->frames[current_frame_index]));
                }
                else {
                    playing = false;
                    current_frame_index = animation_lenght() - 1;
                }
            }
            else {
                set_frame(&(current_animation->frames[current_frame_index]));
            }
        }
    }
}


void AnimatedSprite::render(BatchRenderer & renderer) {
    if (current_frame) {
        current_frame->render(renderer, entity->position.cast_to<float>(), flip_x, flip_y, scale);
    }
}

void AnimatedSprite::connect(const std::string& id, const std::string& next_id)
{
    get(id)->next_id = next_id;
}

void AnimatedSprite::play(const std::string & id, bool restart) {

    if (id != current_id || restart) {

        Log::debug("%s", id.c_str());

        current_id = id;
        current_animation = &(animations[current_id]);
    
        animation_timer = 0.0f;
        current_frame_index = 0;

        set_frame(&(current_animation->frames[current_frame_index]));
    }
    
    playing = true;
}

void AnimatedSprite::stop(){
    playing = false;
}