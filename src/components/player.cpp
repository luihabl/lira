
#include "player.h"

using namespace TinySDL;
using namespace Lira;

void Player::begin()
{
    par = Parameters();
    st = State();
    st.current_gravity = par.gravity;
    st.hp = par.hp_max;
    
    jump.add(Key::Z)
        .register_input();

    dash.add(Key::X)
        .register_input();
    
    up.add(Key::Up)
        .add(Key::W)
        .register_input();

    down.add(Key::Down)
        .add(Key::S)
        .set_repeat(500, 150)
        .register_input();

    horizontal_input.add(Key::Left, Key::Right)
                    .add(Key::A, Key::D)
                    .register_input();

    vertical_input.add(Key::Up, Key::Down)
                    .add(Key::W, Key::S)
                    .register_input();

    animator = get_sibling<AnimatedSprite>();

    animator->connect("intojump", "jump loop");
    animator->connect("wall jump", "jump loop");
    animator->connect("intofall", "fall loop");
    animator->connect("landing", "idle");
    animator->connect("into dash", "dash loop");

    actor = get_sibling<Actor>();
    actor->auto_update = false;

    animation_states.add(anim_jump, [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s) 
        {
            if (s->get_last() == anim_slide)
                a->play("wall jump");
            else
                a->play("intojump");
        });
    animation_states.add(anim_walk, [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s) { a->play("walk");});
    animation_states.add(anim_slide, [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s) {a->play("cling loop"); });
    animation_states.add(anim_fall, [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s) {a->play("intofall"); }, nullptr,
        [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s)
        {
            if (s->get_state() == anim_walk || s->get_state() == anim_idle)
            {
                a->scale = { 1.35f, 0.65f }; // make this depend on the impact velocity
            }
                
        });
    animation_states.add(anim_idle, [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s)
        {
            if (s->get_last() == anim_fall || s->get_last() == anim_jump)
            {
                a->play("landing");
            } 
            else
                a->play("idle");
        });
    animation_states.add(anim_dash, 
    [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s) {
        a->is_visible = false;
        a->play("into dash"); 
    }, 
    nullptr,
    [](AnimatedSprite* a, StateMachine<AnimatedSprite>* s){
        a->is_visible = true;
    });
}

void Player::update()
{
    move();
    animate();
}

void Player::render(BatchRenderer& renderer)
{
    if (st.is_dashing)
        for (auto& trail : trails)
        {
            trail.render(renderer);
        }
}

int Player::get_hp() const
{
    return st.hp;
}

int Player::get_max_hp() const
{
    return par.hp_max;
}

void Player::hit(int amount)
{    
    Game::pause_for(0.1f);
    st.hp = Mathf::clamp(st.hp - amount, 0, par.hp_max);
}

void Player::recover(int amount)
{
    st.hp = Mathf::clamp(st.hp + amount, 0, par.hp_max);
}

void Player::recharge_dash()
{
    st.dash_counter = 0.0f;
}

void Player::move()
{
    Vec2& velocity = actor->velocity;
    
    st.was_on_ground = st.on_ground;
    st.on_ground = actor->on_ground();

    st.was_on_wall = st.on_wall;
    st.on_wall = actor->on_wall(1) || actor->on_wall(-1);

    st.current_gravity = par.gravity;

    if (Mathf::sign(horizontal_input.value()) != 0)
        st.facing = (int) Mathf::sign(horizontal_input.value());

    // Jump


    if (st.on_ground) 
    {
        st.jump_counter = 0;
    }

    if (jump.just_pressed() && st.jump_counter < par.n_jumps) 
    {
        bool on_wall_margin_r = actor->on_wall(par.wall_jump_margin);
        bool on_wall_margin_l = actor->on_wall(-par.wall_jump_margin);
        if ((on_wall_margin_r || on_wall_margin_l) && !st.on_ground)
        {                    
            if (on_wall_margin_r)
            {
                velocity[0] = -par.wall_jump_speed;
                velocity[1] = 1.1f * par.jump_speed;
            }
            else if (on_wall_margin_l)
            {
                velocity[0] = par.wall_jump_speed;
                velocity[1] = 1.1f * par.jump_speed;
            }   
        }
        else
        {
            velocity[1] = par.jump_speed;
        }

        animator->scale = {0.7f, 1.3f};

        Sound::play("Jump");

        st.jump_counter++;
    }

    // Decrease gravity when holding the jump button
    if (jump.pressed()) 
    {
        st.current_gravity *= 0.75f;
    }
    else
    {
        st.current_gravity = par.gravity;
    }

    // Recharge dash after delay
    if (st.on_ground && !st.is_dashing && st.dash_counter >= par.dash_length && !st.is_recharging_dash) 
    {
        st.is_recharging_dash = true;
        entity->add_component(Timer(par.dash_recharge_delay , [this](Timer* self) {
            st.is_recharging_dash = false;
            st.dash_counter = 0.0f;
            self->destroy();
        }));
    }
    
    if (dash.pressed() && st.dash_counter < par.dash_length && !st.invincible)
    {
        

        st.is_dashing = true;
        st.dash_counter += 1.0f;
        st.current_gravity = 0.0f;

        Vec2 dash_direction = { horizontal_input.value(), vertical_input.value() };
        dash_direction = dash_direction.normalized();

        if (dash_direction.length() == 0.0f)
        {
            dash_direction = velocity.normalized();
        }

        actor->velocity += par.dash_accel * dash_direction;

        if (velocity.length() > par.dash_max_speed)
            velocity = velocity.normalized() * par.dash_max_speed;
    }
    else
    {
        st.is_dashing = false;
    }

    // Horizontal movement

    if (!st.is_dashing)
    {
        if (st.on_ground)
            velocity[0] += horizontal_input.value() * par.floor_accel * GameProperties::delta_time();
        else
            velocity[0] += horizontal_input.value() * par.air_accel * GameProperties::delta_time();


        if (abs(velocity[0]) > par.floor_max_speed)
            velocity[0] = Mathf::approach(velocity[0],  par.floor_max_speed * Mathf::sign(velocity[0]), 1000.0f * GameProperties::delta_time());


        if (horizontal_input.value() == 0.0f && st.on_ground)
            velocity[0] = Mathf::approach(velocity[0], 0.0f, par.floor_friction * GameProperties::delta_time());

    }

    // Wall slide

    st.sliding = false;
    if (st.on_wall)
    {
        if (!st.on_ground && velocity[1] >= 0 && horizontal_input.value() != 0)
        {
            st.sliding = true;
            st.current_gravity = par.slide_gravity_multiplier * par.gravity;
            //velocity[1] = std::min(velocity[1], max_slide_speed);
            // Change friction!
            
            st.jump_counter = st.jump_counter > 1 ? 1 : st.jump_counter;
            // jump_counter = 1;
        }
    }

    // Vertical movement
    
    velocity[1] += st.current_gravity * GameProperties::delta_time();

    if (abs(velocity[1]) > par.vertical_max_speed && !st.sliding)
    {
        velocity[1] = Mathf::approach(velocity[1], par.vertical_max_speed * Mathf::sign(velocity[1]), 1000 * GameProperties::delta_time());
    }
    else if (st.sliding)
    {
        velocity[1] = Mathf::approach(velocity[1], par.max_slide_speed, 1000 * GameProperties::delta_time());
    }

    bool was_hit = actor->collider->check_first(Layer::Collision::danger);
    if (was_hit)
    {
        if(!st.invincible)
        {
            hit(1);

            st.invincible_counter = 0;
            st.invincible = true;

            velocity = Vec2({- ((float) st.facing) * 150.0f, -120.0f});

            entity->add_component(Timer(1.0f , [this](Timer* self) {
                st.invincible = false;
                animator->is_visible = true;
                self->destroy();
            }));
        }
    }

    actor->move_x();
    actor->move_y();
}


void Player::animate()
{
    if (horizontal_input.value() != 0 && st.on_ground && actor->velocity[1] >= 0) {
        animation_states.set(anim_walk, animator);
    }
    else if (st.on_ground && actor->velocity[1] >= 0) {
        animation_states.set(anim_idle, animator);
    }

    if (!st.on_ground && !st.is_dashing && !st.sliding) {
        if (actor->velocity[1] < 0) {
            animation_states.set(anim_jump, animator);
        }
        else if (actor->velocity[1] > 0) {
            animation_states.set(anim_fall, animator);
        }
            
    }

    if (st.is_dashing)
    {
        animation_states.set(anim_dash, animator);

        trails.push_back({entity->position, 5.0f, false});

        for (int i = (int)trails.size() - 1; i >= 0; i--)
        {
            if (trails[i].remove)
            {
                trails.erase(trails.begin() + i);
            }
        }
    }
    else
    {

        trails.clear();
    }

    if (st.sliding)
    {
        animator->flip_x = true;
        animation_states.set(anim_slide, animator);
    }
    else
    {
        animator->flip_x = false;
    }

    if (st.invincible)
    {
        if (st.invincible_counter % 4 == 1)
            animator->is_visible = !animator->is_visible;

        st.invincible_counter++;
    }


    animator->scale[0] = std::abs(animator->scale[0]) * (float)st.facing;
    animator->scale = Mathf::approach(animator->scale, {(float)st.facing, 1.0f}, GameProperties::delta_time() * 4.0f);
}

const Player::State& Player::state()
{
    return st;
}

const Player::Parameters& Player::parameters()
{
    return par;
}
