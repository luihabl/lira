#include "gui.h"

#include <auk.h>

#include "imgui.h" 
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "../level.h"
#include "../components/player.h"
#include "../components/actor.h"

using namespace Lira;
using namespace auk;

void style();

namespace
{
    Player::Parameters saved_parameters = Player::Parameters();
    bool par_initialized = false;
}

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
    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::draw(LiraGame* game)
{

    Level* level = (Level*) game->get_current_scene();

    //ImGui::SetNextWindowBgAlpha(0.5f);
    ImGui::Begin("Lira debug");


        if (ImGui::CollapsingHeader("Game"))
        {
            ImGui::Text("FPS: %.1f ", ImGui::GetIO().Framerate);
        }

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
                // player_par = (Player::Parameters*) &player->parameters();
                if (!par_initialized)
                {
                    saved_parameters = player->parameters();
                    par_initialized = true;
                }

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

        #define IMGUI_FSLIDE(var, v0, v1) ImGui::SliderFloat(#var, &saved_parameters.var, v0, v1);
        #define IMGUI_ISLIDE(var, v0, v1) ImGui::SliderInt(#var, &saved_parameters.var, v0, v1);

        if (ImGui::CollapsingHeader("Scene"))
        {

            if (ImGui::TreeNode("Room"))
            {
                ImGui::Text("Camera position: [%.1f, %.1f]", level->camera[0], level->camera[1]);
                ImGui::Text("Camera rendered position: [%d, %d]", (int) roundf(level->camera[0]), (int) roundf(level->camera[1]));
                
                ImGui::Spacing();

                ImGui::BulletText("Current room\n"
                "Number: %lu\n"
                "BBBox: [x:%d, y:%d, w:%d, h:%d]", 
                level->current_room.id, level->current_room.bbox.x, level->current_room.bbox.y, level->current_room.bbox.w, level->current_room.bbox.h);
                ImGui::Spacing();

                ImGui::BulletText("Next room\n"
                "Number: %lu\n"
                "BBox: [x:%d, y:%d, w:%d, h:%d]", 
                level->next_room.id, level->next_room.bbox.x, level->next_room.bbox.y, level->next_room.bbox.w, level->next_room.bbox.h);
                ImGui::Spacing();

                ImGui::Spacing();

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Update and rendering"))
            {
                ImGui::Text("Active entities: %lu", level->get_entities().size());

                size_t total_rendered_components = 0;
                if (ImGui::BeginTable("Rendering layers", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
                {
                    ImGui::TableSetupColumn("Layer", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("Components", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableHeadersRow();
                    for (const auto& [id, layer] : level->get_render_layers())
                    {
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%d", id);

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%lu", layer.components.size());
                        total_rendered_components += layer.components.size();
                    }

                    ImGui::EndTable();
                }

                ImGui::Text("Rendered components: %lu", total_rendered_components);

                size_t total_updated_components = 0;
                if (ImGui::BeginTable("Components", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
                {
                    ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Components", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableHeadersRow();
                    for (const auto& [id, items] : level->get_components())
                    {
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%lu", id);

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%lu", items.size());
                        total_updated_components += items.size();
                    }

                    ImGui::EndTable();
                }

                ImGui::Text("Updated components: %lu", total_updated_components);

                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Inventory"))
            {

                auto* inventory = level->inventory.get_all();
                
                if (ImGui::BeginTable("Components", 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
                {
                    ImGui::TableSetupColumn("Item", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Amount", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableHeadersRow();
                    for (const auto& [name, items] : *inventory)
                    {
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%s", name.c_str());

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%lu", items.size());
                    }

                    ImGui::EndTable();
                }

                ImGui::TreePop();
            }


        }

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

                ImGui::Spacing();

                if(ImGui::Button("Reset"))
                {
                    player->reset();
                }

                ImGui::TreePop();
                ImGui::Separator();
            }

            if (ImGui::TreeNode("Parameters"))
            {
                ImGui::Text("Changes will not be saved");

                if (true)
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

                    ImGui::Spacing();

                    if(ImGui::Button("Reset"))
                    {
                        saved_parameters = Player::Parameters();
                    }

                    ImGui::TreePop();
                    ImGui::Separator();
                }
            }

            ImGui::PopItemWidth();
        }

        player->set_parameters(saved_parameters, true);
                

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