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

void style();

void GUI::init()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui_ImplSDL2_InitForOpenGL(Window::get_window(), Window::get_context());
    ImGui_ImplOpenGL3_Init("#version 330");

    ::style();
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
    //ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::draw(LiraGame* game)
{

    Level* level = (Level*) game->get_current_scene();

    //ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::Begin("Lira debug");

        ImGui::Text("FPS: %.1f ", ImGui::GetIO().Framerate);

        Vec2 player_velocity = Vec2::zeros;

        Player::State player_state;
        Player::Parameters* player_par = nullptr;

        player_state.hp = -1;
        player_state.current_gravity = -1;
        Player* player = nullptr;

        if(level)
        {
            player = level->get_first<Player>();
            
            if(player)
            {
                player_state = player->state();
                player_par = (Player::Parameters*) &player->parameters();

                auto* actor = player->get_sibling<Actor>();
                if(actor)
                {
                    player_velocity = actor->velocity;
                }
            }
        }

        #define IMGUI_STATUS_FLOAT(var) ImGui::Text(#var ": %.2f ", player_state.var);
        #define IMGUI_STATUS_INT(var) ImGui::Text(#var ": %d ", player_state.var);
        #define IMGUI_STATUS_BOOL(var) ImGui::Text(#var ": %s ", player_state.var ? "true" : "false");

        #define IMGUI_FSLIDE(var, v0, v1) ImGui::SliderFloat(#var, &player_par->var, v0, v1);
        #define IMGUI_ISLIDE(var, v0, v1) ImGui::SliderInt(#var, &player_par->var, v0, v1);

        if (ImGui::CollapsingHeader("Player"))
        {
            ImGui::PushItemWidth(-ImGui::GetContentRegionAvail().x * 0.5f);
            if (ImGui::TreeNode("State"))
            {
                ImGui::Text("in_scene: %s ", player ? "true" : "false");

                IMGUI_STATUS_FLOAT(current_gravity);
                IMGUI_STATUS_INT(dash_recharge_counter);
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

                ImGui::Text("velocity: [%.0f, %.0f] ", player_velocity[0], player_velocity[1]);

                ImGui::TreePop();
                ImGui::Separator();
            }

            if (ImGui::TreeNode("Parameters"))
            {
                ImGui::Text("Changes will not be saved");

                if (player_par)
                {
                    IMGUI_FSLIDE(floor_max_speed, 0, 250);
                    IMGUI_FSLIDE(vertical_max_speed, 0, 1000);
                    IMGUI_FSLIDE(floor_accel, 0, 2000);
                    IMGUI_FSLIDE(floor_friction, 0, 2000);
                    IMGUI_FSLIDE(max_slide_speed, 0, 200);

                    ImGui::Spacing();

                    IMGUI_ISLIDE(dash_recharge_count, 0, 200);
                    IMGUI_FSLIDE(dash_length, 0, 40);
                    IMGUI_FSLIDE(dash_max_speed, 0, 500);
                    IMGUI_FSLIDE(dash_accel, 0, 500);
                    IMGUI_FSLIDE(dash_recharge_delay, 0, 1);

                    ImGui::Spacing();

                    IMGUI_FSLIDE(gravity, 0, 1000);

                    ImGui::Spacing();

                    IMGUI_FSLIDE(jump_gravity_factor, 0, 1);
                    IMGUI_FSLIDE(air_accel, 0, 700);
                    IMGUI_FSLIDE(jump_speed, -400, 0);
                    IMGUI_FSLIDE(wall_jump_speed, 0, 300);
                    IMGUI_ISLIDE(n_jumps, 0, 5);
                    IMGUI_ISLIDE(wall_jump_margin, 0, 5);

                    ImGui::Spacing();

                    IMGUI_FSLIDE(slide_gravity_multiplier, 0, 1);

                    ImGui::Spacing();

                    IMGUI_ISLIDE(hp_max, 1, 10);

                    ImGui::TreePop();
                    ImGui::Separator();
                }
            }

            ImGui::PopItemWidth();
        }

        
                

    ImGui::End();
}


void style()
{
    auto& style = ImGui::GetStyle();

    style.WindowRounding = 4;
    style.ChildRounding = 4;
    style.FrameRounding = 4;
    style.PopupRounding = 4;
    style.ScrollbarRounding = 4;
    style.TabRounding = 4;
    style.GrabRounding = 4;

    style.FrameBorderSize = 1;
    style.TabBorderSize = 1;

    ImVec4 cnorm = ImVec4(0.28f, 0.27f, 0.25f, 0.40f);
    ImVec4 cnorm_hovered = ImVec4(0.40f, 0.37f, 0.34f, 1.00f);
    ImVec4 cnorm_active = ImVec4(0.58f, 0.49f, 0.41f, 1.00f);

    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(0.79f, 0.91f, 0.87f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.09f, 0.09f, 0.09f, 0.94f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.29f, 0.27f, 0.25f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = cnorm;
    colors[ImGuiCol_FrameBgHovered] = cnorm_hovered;
    colors[ImGuiCol_FrameBgActive] = cnorm_active;
    colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.64f, 0.30f, 0.00f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.45f, 0.21f, 0.00f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.28f, 0.27f, 0.25f, 0.40f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.40f, 0.37f, 0.34f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.58f, 0.49f, 0.41f, 1.00f);
    colors[ImGuiCol_Header] = cnorm;
    colors[ImGuiCol_HeaderHovered] = cnorm_hovered;
    colors[ImGuiCol_HeaderActive] = cnorm_active;
    colors[ImGuiCol_Separator] = ImVec4(0.50f, 0.46f, 0.43f, 0.50f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.75f, 0.41f, 0.10f, 0.78f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.41f, 0.10f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.38f, 0.18f, 0.00f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.72f, 0.34f, 0.00f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.91f, 0.43f, 0.00f, 0.95f);
    colors[ImGuiCol_Tab] = ImVec4(0.22f, 0.22f, 0.22f, 0.86f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.33f, 0.30f, 0.28f, 0.80f);
    colors[ImGuiCol_TabActive] = ImVec4(0.25f, 0.22f, 0.19f, 0.94f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.11f, 0.07f, 0.97f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.42f, 0.27f, 0.14f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.60f, 0.26f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.98f, 0.60f, 0.26f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}