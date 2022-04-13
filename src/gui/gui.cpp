#include "gui.h"

#include <tinysdl.h>

#include "imgui.h" 
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "../level.h"
#include "../components/player.h"
#include "../components/actor.h"

using namespace Lira;
using namespace TinySDL;

void GUI::init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(Window::get_window(), Window::get_context());
    ImGui_ImplOpenGL3_Init("#version 330");
}

void GUI::end()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void GUI::input(SDL_Event& event)
{
    ImGui_ImplSDL2_ProcessEvent(&event);
}

void GUI::render(LiraGame* game)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(Window::get_window());
    ImGui::NewFrame();

    GUI::draw(game);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::draw(LiraGame* game)
{

    Level* level = (Level*) game->get_current_scene();

    ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::Begin("Lira debug");

        ImGui::Text("FPS: %.1f ", ImGui::GetIO().Framerate);



        Vec2 player_velocity = Vec2::zeros;

        Player::State player_state;
        player_state.hp = -1;
        player_state.current_gravity = -1;
        Player* player = nullptr;

        if(level)
        {
            player = level->get_first<Player>();
            
            if(player)
            {
                player_state = player->state();

                auto* actor = player->get_sibling<Actor>();
                if(actor)
                {
                    player_velocity = actor->velocity;
                }

                // player_dash_charge = player

            }
            
        }

        ImGui::Text("Player in scene: %s ", player ? "true" : "false");

        #define IMGUI_STATUS_FLOAT(var) ImGui::Text(#var ": %.2f ", player_state.var);
        #define IMGUI_STATUS_INT(var) ImGui::Text(#var ": %d ", player_state.var);
        #define IMGUI_STATUS_BOOL(var) ImGui::Text(#var ": %s ", player_state.var ? "true" : "false");

        IMGUI_STATUS_FLOAT(current_gravity);
        IMGUI_STATUS_FLOAT(dash_counter);        
        IMGUI_STATUS_INT(jump_counter);
        IMGUI_STATUS_INT(invincible_counter);
        IMGUI_STATUS_BOOL(on_ground);
        IMGUI_STATUS_BOOL(was_on_ground);
        IMGUI_STATUS_BOOL(on_wall);
        IMGUI_STATUS_BOOL(was_on_wall);
        IMGUI_STATUS_BOOL(is_dashing);
        IMGUI_STATUS_BOOL(is_recharging_dash);
        IMGUI_STATUS_BOOL(sliding);
        IMGUI_STATUS_BOOL(invincible);

        IMGUI_STATUS_INT(facing);        
        IMGUI_STATUS_INT(hp);

        // ImGui::Text("Player velocity: [%.0f, %.0f] ", player_velocity[0], player_velocity[1]);
                

    ImGui::End();
}