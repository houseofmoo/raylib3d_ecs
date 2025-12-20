#include "debug/debug_window.h"

#include "rlImGui.h"
#include "data/game/game.h"
#include "data/player/player.h"
#include "utils/debug.h"
#include "components/components.h"

namespace debug {
    static bool show_demo_window = false;
    
    void SetImGuiDocking(ImGuiIO& io) {
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            // Fullscreen invisible dockspace window
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);

            ImGuiWindowFlags host_window_flags =
                ImGuiWindowFlags_NoTitleBar |
                ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize |
                ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus |
                ImGuiWindowFlags_NoNavFocus |
                ImGuiWindowFlags_NoBackground |
                ImGuiWindowFlags_MenuBar;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

            ImGui::Begin("DockSpaceHost", nullptr, host_window_flags);

            ImGui::PopStyleVar(3);

            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
            ImGui::DockSpace(dockspace_id, ImVec2(0, 0), dockspace_flags);

            ImGui::End();
        }
    }
    

    void DrawDebugUI(Storage::Registry& world, ImGuiIO& io) {
        static bool show_demo_window = false;
        static float position_x = 0;
        static float position_z = 0;
        static bool position_blocked = false;

        rlImGuiBegin();
        SetImGuiDocking(io);
        // State& state = GetState_Debug();

        ImGui::Begin("Debug Panel");

        ImGui::Checkbox("Demo window", &show_demo_window);
        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        ImGui::Separator();
            
        ImGui::Text("FPS: %d", GetFPS());
        ImGui::Text("Entities: %d", data::game::g_number_entities);
        ImGui::Text("Enemies: %d", data::game::g_number_enemies);
        ImGui::Separator();

        if (ImGui::SliderInt("Difficulty", &data::game::g_difficulty_level, 0, 1000)) {}
        ImGui::Text("Spawn time: %.2f", data::game::g_enemy_spawn_interval);
        ImGui::Text("Enemy hp: %d", (data::game::g_difficulty_level / 10) + 30);
        ImGui::Separator();

        ImGui::Text("Id: %d", data::player::g_player.id);
        ImGui::Text("Level: %d", data::player::g_player.level);
        ImGui::Text("Exp: %d", data::player::g_player.exp);
        ImGui::Text("ExpLvl: %d", data::player::g_player.exp_to_next_level);
        ImGui::Separator();

        float exp = static_cast<float>(data::player::g_player.exp);
        float next = static_cast<float>(data::player::g_player.exp_to_next_level);
        ImGui::ProgressBar(exp/next, ImVec2(-FLT_MIN, 0.0f), "Exp");
        ImGui::Text("Dmg%: %.2f", data::player::g_player.damage_multiplier);
        ImGui::Text("ASpd%: %.2f", data::player::g_player.attack_speed_multiplier);
        ImGui::Text("Mspd%: %.2f", data::player::g_player.move_speed_multiplier);
        ImGui::Text("PRng%: %.2f", data::player::g_player.pickup_range_multiplier);
        ImGui::Separator();

        ImGui::Checkbox("God Mode: ", &data::player::g_player.god_mode);
        ImGui::Checkbox("Loot Mode: ", &data::player::g_player.always_drop_loot);
        ImGui::Separator();

        for (auto wep : world.View<tag::Weapon, cmpt::WeaponStats>()) {
            auto& stats = world.GetComponent<cmpt::WeaponStats>(wep);
            ImGui::Text("Weapon1:");
            ImGui::Text("  dmg: %d", stats.damage);
            //ImGui::Text("  pen: %d", stats.penetration);
            //ImGui::Text("  cnt: %d", stats.pellet_count);
            ImGui::Separator();
        }

        if (ImGui::Button("Draw Tilemap")) {
            data::game::terrain.DrawTileMap();
        }


        ImGui::InputFloat("X:", &position_x);
        ImGui::InputFloat("Z:", &position_z);
        if (ImGui::Button("Test Point")) {
            position_blocked = data::game::terrain.IsBlockedWorld(position_x, position_z);
        }
        ImGui::Text("Blocked: %d", position_blocked);

        #ifdef PROFILER_ENABLED
        ImGui::Separator();
        if (ImGui::Checkbox("Enable Profiler: ", &data::player::g_player.profiler_enabled)) {
            PRINT("Profiler contents cleared");
            PROFILER_CLEAR;
        }

        if (ImGui::Button("Dump profiler", ImVec2{120, 20})) {
            PRINT("Profiler contents written");
            PROFILER_PRINT;
        }
        if (ImGui::Button("Clear profiler", ImVec2{120, 20})) {
            PRINT("Profiler contents cleared");
            PROFILER_CLEAR;
        }
        #endif

        ImGui::End();
        rlImGuiEnd();
    }
}