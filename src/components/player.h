#pragma once

#include <tinysdl.h>
#include "../core/ecs.h"
#include "../input/virtualbutton.h"
#include "../input/virtualaxis.h"
#include "../util/state_machine.h"
#include "../modules/game.h"
#include "../sound/sound.h"

#include "../layers.h"

#include "animated_sprite.h"
#include "actor.h"
#include "timer.h"


namespace Lira {

    class Player : public Component 
    {   
    public:

        Player() = default;

        void begin() override;
        void update() override;
        void render(TinySDL::BatchRenderer& renderer) override;

        int get_hp() const;
        int get_max_hp() const;
        void hit(int amount);
        void recover(int amount);
        void recharge_dash();

        // Constants
        struct Parameters
        {
            float floor_max_speed = 85.0f;
            float vertical_max_speed = 340.0f;
            float floor_accel = 700.0f;
            float floor_friction = 650.0f;
            float max_slide_speed = 50.0f;

            float dash_length = 15.0f;
            float dash_max_speed = 220.0f;
            float dash_accel = 150.0f;
            float dash_recharge_delay = 0.2f;
            
            float gravity = 600.0f;

            float air_accel = 300.0f;
            float jump_speed = -150.0f;
            float wall_jump_speed = 170.0f;
            int n_jumps = 2;
            int wall_jump_margin = 3;

            float slide_gravity_multiplier = 0.18f;

            int hp_max = 2;
        };

        // State variables
        struct State
        {
            float current_gravity;
            
            float dash_counter = 0.0f;        
            int jump_counter = 0;
            int invincible_counter = 0;
            
            bool on_ground = true;
            bool was_on_ground = true;
            bool on_wall = false;
            bool was_on_wall = false;
            bool is_dashing = false;
            bool is_recharging_dash = false;
            bool sliding = false;
            bool invincible = false;

            int facing = 1;        
            int hp;
        };
        
    private:

        VirtualButton right, left, up, down, jump, dash;
        VirtualAxis horizontal_input, vertical_input;
        AnimatedSprite * animator;
        Actor * actor;
        Parameters par;
        State st;

        StateMachine<AnimatedSprite> animation_states;
        static constexpr size_t anim_jump = 0;
        static constexpr size_t anim_walk = 1;
        static constexpr size_t anim_slide = 2;
        static constexpr size_t anim_fall = 3;
        static constexpr size_t anim_idle = 4;
        static constexpr size_t anim_dash = 5;
        static constexpr size_t anim_hurt = 6;
        
        struct Trail
        {
            TinySDL::IVec2 pos;
            float radius;
            bool remove = false;

            void render(TinySDL::BatchRenderer& renderer)
            {
                renderer.draw_circle_fill(pos.cast_to<float>() + TinySDL::Vec2({0.0f, -6.0f}), radius, TinySDL::Color::white);
                radius -= 0.75f;
                if (radius <= 0)
                    remove = true;
            }
        };
        
        std::vector<Trail> trails;

        void move();
        void animate();

    public:
        const State& state();
        const Parameters& parameters();
    };

}
