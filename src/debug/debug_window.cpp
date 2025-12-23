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
   
    void DrawStatsTab(Storage::Registry& world) {
        ImGui::Text("fps:        %d", GetFPS());
        ImGui::Text("entities:   %d", data::g_game.entity_count);
        ImGui::Text("enemies:    %d", data::g_game.enemy_count);
        ImGui::Text("spawn time: %.2f", data::g_game.enemy_spawn_interval);
        ImGui::Text("difficulty: %d", data::g_game.difficulty);
        if (ImGui::SliderInt("##diff_slider", &data::g_game.difficulty, 0, 2000)) {}
        ImGui::Separator();

        ImGui::Text("id:         %d", data::g_player.id);
        ImGui::Text("level:      %d", data::g_player.level);
        ImGui::Text("gold:       %d", data::g_player.money);
        ImGui::Text("exp:        %d", data::g_player.exp);
        ImGui::Text("explvl:     %d", data::g_player.exp_to_next_level);
        ImGui::Separator();

        ImGui::Text("dmg%:       %.2f", data::g_player.damage_multiplier);
        ImGui::Text("a-spd%:     %.2f", data::g_player.attack_speed_multiplier);
        ImGui::Text("m-spd%:     %.2f", data::g_player.move_speed_multiplier);
        ImGui::Text("d-rng%:     %.2f", data::g_player.move_speed_multiplier);
        ImGui::Text("pu-rng%:    %.2f", data::g_player.pickup_range_multiplier);
        ImGui::Separator();

        ImGui::Checkbox(" god mode", &data::g_player.god_mode);
        ImGui::Checkbox(" loot mode", &data::g_player.always_drop_loot);
        ImGui::Separator();

        // tilemap testing
        if (ImGui::Button("draw tilemap")) {
            data::g_terrain.DrawTileMap();
        }
        // ImGui::InputFloat("X:", &position_x);
        // ImGui::InputFloat("Z:", &position_z);
        // if (ImGui::Button("Test Point")) {
        //     position_blocked = data::game::terrain.IsBlockedWorld(position_x, position_z);
        // }
        // ImGui::Text("Blocked: %d", position_blocked);
    }

    void DrawWeapsTab(Storage::Registry& world) {
         if (auto* wep = world.TryGetComponent<cmpt::Pistol>(data::g_player.id)) {
            ImGui::Text("pistol:");
            ImGui::Text("  dmg:     %d", wep->base_stats.damage);
            ImGui::Text("  pjk spd: %.2f", wep->base_stats.projectile_speed);
            ImGui::Text("  cd:      %.2f", wep->base_stats.cooldown);
            ImGui::Separator();
        }

        if (auto* wep = world.TryGetComponent<cmpt::Shotgun>(data::g_player.id)) {
            ImGui::Text("shotgun:");
            ImGui::Text("  dmg:     %d", wep->base_stats.damage);
            ImGui::Text("  pjk spd: %.2f", wep->base_stats.projectile_speed);
            ImGui::Text("  cd:      %.2f", wep->base_stats.cooldown);
            ImGui::Text("  sprd:    %.2f", wep->spread_deg);
            ImGui::Text("  plt cnt: %d", wep->pellet_count);
            ImGui::Separator();
        }

        if (auto* wep = world.TryGetComponent<cmpt::Grenade>(data::g_player.id)) {
            ImGui::Text("grenade:");
            ImGui::Text("  dmg:     %d", wep->base_stats.damage);
            ImGui::Text("  pjk spd: %.2f", wep->base_stats.projectile_speed);
            ImGui::Text("  cd:      %.2f", wep->base_stats.cooldown);
            ImGui::Separator();
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

        // exmaples so i can figure shit out
        ImGui::Checkbox("Demo window", &show_demo_window);
        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }
        ImGui::Separator();

        // main info panel
        if (ImGui::BeginTabBar("main")) {
            // Stats Tab
            if (ImGui::BeginTabItem("stats")) {
                DrawStatsTab(world);
                ImGui::EndTabItem();
            }

            // Weapons Tab
            if (ImGui::BeginTabItem("weapons")) {
                DrawWeapsTab(world);
                ImGui::EndTabItem();
            }

            #ifdef PROFILER_ENABLED
            // Profiler Tab
            if (ImGui::BeginTabItem("profiler")) {
                if (ImGui::Checkbox("enable profiler: ", &data::g_player.profiler_enabled)) {
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
                ImGui::EndTabItem();
            }
            #endif
            ImGui::EndTabBar();
        }
        ImGui::End();
        rlImGuiEnd();
    }
}