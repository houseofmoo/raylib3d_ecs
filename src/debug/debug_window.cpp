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
            
        ImGui::Text("fps:        %d", GetFPS());
        ImGui::Text("entities:   %d", data::game::g_number_entities);
        ImGui::Text("enemies:    %d", data::game::g_number_enemies);
        ImGui::Text("spawn time: %.2f", data::game::g_enemy_spawn_interval);
        ImGui::Text("enemy hp:   %d", (data::game::g_difficulty_level / 10) + 30);
        ImGui::Text("difficulty: %d", data::game::g_difficulty_level);
        if (ImGui::SliderInt("##diff_slider", &data::game::g_difficulty_level, 0, 2000)) {}
        ImGui::Separator();

        ImGui::Text("id:         %d", data::player::g_player.id);
        ImGui::Text("level:      %d", data::player::g_player.level);
        ImGui::Text("gold:       %d", data::player::g_player.money);
        ImGui::Text("exp:        %d", data::player::g_player.exp);
        ImGui::Text("explvl:     %d", data::player::g_player.exp_to_next_level);
        ImGui::Separator();

        ImGui::Text("dmg%:       %.2f", data::player::g_player.damage_multiplier);
        ImGui::Text("a-spd%:     %.2f", data::player::g_player.attack_speed_multiplier);
        ImGui::Text("m-spd%:     %.2f", data::player::g_player.move_speed_multiplier);
        ImGui::Text("d-rng%:     %.2f", data::player::g_player.move_speed_multiplier);
        ImGui::Text("pu-rng%:    %.2f", data::player::g_player.pickup_range_multiplier);
        ImGui::Separator();

        ImGui::Checkbox(" god mode", &data::player::g_player.god_mode);
        ImGui::Checkbox(" loot mode", &data::player::g_player.always_drop_loot);
        ImGui::Separator();

        for (auto wep : world.View<tag::Weapon, cmpt::WeaponStats>()) {
            auto& stats = world.GetComponent<cmpt::WeaponStats>(wep);
            ImGui::Text("wep-pistol:");
            ImGui::Text("  dmg: %d", stats.damage);
            //ImGui::Text("  pen: %d", stats.penetration);
            //ImGui::Text("  cnt: %d", stats.pellet_count);
            ImGui::Separator();
        }

        // tilemap testing
        if (ImGui::Button("draw tilemap")) {
            data::game::terrain.DrawTileMap();
        }
        // ImGui::InputFloat("X:", &position_x);
        // ImGui::InputFloat("Z:", &position_z);
        // if (ImGui::Button("Test Point")) {
        //     position_blocked = data::game::terrain.IsBlockedWorld(position_x, position_z);
        // }
        // ImGui::Text("Blocked: %d", position_blocked);

        #ifdef PROFILER_ENABLED
        ImGui::Separator();
        if (ImGui::Checkbox("enable profiler: ", &data::player::g_player.profiler_enabled)) {
            PRINT("Profiler contents cleared");
            PROFILER_CLEAR;
        }

        if (ImGui::Button("dump profiler", ImVec2{120, 20})) {
            PRINT("Profiler contents written");
            PROFILER_PRINT;
        }
        if (ImGui::Button("clear profiler", ImVec2{120, 20})) {
            PRINT("Profiler contents cleared");
            PROFILER_CLEAR;
        }
        #endif

        ImGui::End();
        rlImGuiEnd();
    }
}