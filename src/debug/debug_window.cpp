#include "debug/debug_window.h"

#include "rlImGui.h"
#include "data/game/game.h"
#include "data/player/player.h"
#include "utils/debug.h"
#include "components/components.h"
#include "spawners/world/enemies/grunt.h"
#include "spawners/world/enemies/brute.h"
#include "spawners/equip/weapon/pistol.h"
#include "spawners/equip/weapon/shotgun.h"
#include "spawners/equip/weapon/rifle.h"
#include "spawners/equip/weapon/grenade_launcher.h"


namespace debug {
    
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
    
    void DrawStatsTab(strg::Registry& world) {
        ImGui::Text("fps:        %d", GetFPS());
        ImGui::Text("entities:   %d", data::g_game.entity_count);
        ImGui::Text("enemies:    %d", data::g_game.enemy_count);
        ImGui::Text("spawn time: %.2f", data::g_game.enemy_spawn_interval);
        ImGui::Text("difficulty: %d", data::g_game.difficulty);
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
        ImGui::Text("d-rng%:     %.2f", data::g_player.dash_range_multiplier);
        ImGui::Text("pu-rng%:    %.2f", data::g_player.pickup_range_multiplier);
        ImGui::Separator();

        // add stats
        ImVec2 bsize = ImVec2{100, 30};
        if (ImGui::Button("+exp", bsize)) {
            data::g_player.exp += data::cnst::EXP_VALUE;
        }
        ImGui::SameLine();
        if (ImGui::Button("+mny", bsize)) {
            data::g_player.money += data::cnst::MONEY_VALUE;
        }

        if (ImGui::Button("+dmg", bsize)) {
            data::g_player.damage_multiplier += data::cnst::DAMAGE_POWERUP_VALUE;
        }
        ImGui::SameLine();
        if (ImGui::Button("-dmg", bsize)) {
            data::g_player.damage_multiplier -= data::cnst::DAMAGE_POWERUP_VALUE;
        }

        if (ImGui::Button("+atkspd", bsize)) {
            data::g_player.attack_speed_multiplier += data::cnst::ATTACK_SPEED_POWERUP_VALUE;
        }
        ImGui::SameLine();
        if (ImGui::Button("-atkspd", bsize)) {
            data::g_player.attack_speed_multiplier -= data::cnst::ATTACK_SPEED_POWERUP_VALUE;
        }

        if (ImGui::Button("+mspd", bsize)) {
            data::g_player.move_speed_multiplier += data::cnst::MOVE_SPEED_POWERUP_VALUE;
        }
        ImGui::SameLine();
        if (ImGui::Button("-mspd", bsize)) {
            data::g_player.move_speed_multiplier -= data::cnst::MOVE_SPEED_POWERUP_VALUE;
        }

        if (ImGui::Button("+prng", bsize)) {
            data::g_player.pickup_range_multiplier += data::cnst::PICKUP_RANGE_POWERUP_VALUE;
        }
        ImGui::SameLine();
        if (ImGui::Button("-prng", bsize)) {
            data::g_player.pickup_range_multiplier -= data::cnst::PICKUP_RANGE_POWERUP_VALUE;
        }

        if (ImGui::Button("+dash", bsize)) {
            data::g_player.dash_range_multiplier += data::cnst::DASH_DISTANCE_POWERUP_VALUE;
        }
        ImGui::SameLine();
        if (ImGui::Button("-dash", bsize)) {
            data::g_player.dash_range_multiplier -= data::cnst::DASH_DISTANCE_POWERUP_VALUE;
        }
    }

    void DrawWeapsTab(strg::Registry& world) {
        ImVec2 size = ImVec2{100, 30};
        if (ImGui::Button("+pistol", size)) {
            spwn::weapon::EquipPistol(world, data::g_player.id);
        }
        ImGui::SameLine();
        if (ImGui::Button("-pistol", size)) {
            spwn::weapon::DequipPistol(world, data::g_player.id);
        }

        if (ImGui::Button("+shotgun", size)) {
            spwn::weapon::EquipShotgun(world, data::g_player.id);
        }
        ImGui::SameLine();
        if (ImGui::Button("-shotgun", size)) {
            spwn::weapon::DequipShotgun(world, data::g_player.id);
        }

        if (ImGui::Button("+rifle", size)) {
            spwn::weapon::EquipRifle(world, data::g_player.id);
        }
        ImGui::SameLine();
        if (ImGui::Button("-rifle", size)) {
            spwn::weapon::DequipRifle(world, data::g_player.id);
        }
        
        if (ImGui::Button("+grenade", size)) {
            spwn::weapon::EquipGrenadeLauncher(world, data::g_player.id);
        }
        ImGui::SameLine();
        if (ImGui::Button("-grenade", size)) {
            spwn::weapon::DequipGrenadeLauncher(world, data::g_player.id);
        }
        ImGui::Separator();

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

        if (auto* wep = world.TryGetComponent<cmpt::Rifle>(data::g_player.id)) {
            ImGui::Text("rifle:");
            ImGui::Text("  dmg:     %d", wep->base_stats.damage);
            ImGui::Text("  pjk spd: %.2f", wep->base_stats.projectile_speed);
            ImGui::Text("  cd:      %.2f", wep->base_stats.cooldown);
            ImGui::Text("  burst:   %d", wep->burst_completed);
            ImGui::Separator();
        }

        if (auto* wep = world.TryGetComponent<cmpt::GrenadeLauncher>(data::g_player.id)) {
            ImGui::Text("grenade:");
            ImGui::Text("  dmg:     %d", wep->base_stats.damage);
            ImGui::Text("  pjk spd: %.2f", wep->base_stats.projectile_speed);
            ImGui::Text("  cd:      %.2f", wep->base_stats.cooldown);
            ImGui::Separator();
        }
    }

    void DrawCheatsTab(strg::Registry& world) {
        // exmaples so i can figure shit out
        static bool show_demo_window = false;
        ImGui::Checkbox(" demo window", &show_demo_window);
        if (show_demo_window) {
            ImGui::ShowDemoWindow(&show_demo_window);
        }

        // cheats
        ImGui::Checkbox(" god mode", &data::g_cheats.god_mode);
        ImGui::Checkbox(" dont spawn enemies", &data::g_cheats.dont_spawn_enemies);
        ImGui::Checkbox(" always loot", &data::g_cheats.always_drop_loot);
        ImGui::Checkbox(" no loot", &data::g_cheats.never_drop_loot);
        ImGui::Separator();

        ImGui::Text("difficulty slider");
        if (ImGui::SliderInt("##diff_slider", &data::g_game.difficulty, 0, 2000)) {}
        ImGui::Separator();

        // spawn enemies
        if (ImGui::Button("spawn grunt line")) {
            for (int x = -10; x < 10; x+= 2) {
                spwn::enemy::Grunt(world, Vector3{(float)x, 0.0f, 0.0f}, cmpt::AIMoveMode::None, 500);
            }
        }
        if (ImGui::Button("spawn brute line")) {
            for (int x = -10; x < 10; x+= 4) {
                spwn::enemy::Brute(world, Vector3{(float)x, 0.0f, 0.0f}, cmpt::AIMoveMode::None, 500);
            }
        }
    }

    void DrawTilemapTab() {
        // tilemap testing
        if (ImGui::Button("draw tilemap")) {
            data::g_terrain.DrawTileMap();
        }
    }

    void DrawProfilerTab() {
        #ifdef PROFILER_ENABLED
        // Profiler Tab
        if (ImGui::BeginTabItem("profiler")) {
            if (ImGui::Checkbox("enable profiler: ", &data::g_cheats.profiler_enabled)) {
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
    }

    void DrawDebugUI(strg::Registry& world, ImGuiIO& io) {
        static bool show_demo_window = false;
        static float position_x = 0;
        static float position_z = 0;
        static bool position_blocked = false;

        rlImGuiBegin();
        SetImGuiDocking(io);
        // State& state = GetState_Debug();
        ImGui::Begin("Debug Panel");
        ImGui::Separator();

        // main info panel
        if (ImGui::BeginTabBar("main")) {
            // stats Tab
            if (ImGui::BeginTabItem("stats")) {
                DrawStatsTab(world);
                ImGui::EndTabItem();
            }

            // weapons Tab
            if (ImGui::BeginTabItem("weapons")) {
                DrawWeapsTab(world);
                ImGui::EndTabItem();
            }

            // cheats tab
            if (ImGui::BeginTabItem("cheats")) {
                DrawCheatsTab(world);
                ImGui::EndTabItem();
            }

            // tilemap tab
            if (ImGui::BeginTabItem("tilemap")) {
                DrawTilemapTab();
                ImGui::EndTabItem();
            }

            DrawProfilerTab();
            ImGui::EndTabBar();
        }
        ImGui::End();
        rlImGuiEnd();
    }
}