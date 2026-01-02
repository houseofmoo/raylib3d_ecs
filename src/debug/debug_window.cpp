#include "debug/debug_window.h"

#include "rlImGui.h"
#include "data/global_data.h"
#include "utils/debug.h"
#include "components/components.h"
#include "spawners/world/enemies/enemies.h"
#include "spawners/equip/weapon/weapons.h"


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
        auto* info = world.TryGetComponent<cmpt::Player>(gd::player.id);
        auto* stats = world.TryGetComponent<cmpt::Stats>(gd::player.id);

        ImGui::Text("fps:        %d", GetFPS());
        ImGui::Text("entities:   %d", gd::game.entity_count);
        ImGui::Text("enemies:    %d", gd::game.enemy_count);
        ImGui::Text("spawn time: %.2f", gd::game.enemy_spawn_interval);
        ImGui::Text("difficulty: %d", gd::game.difficulty);
        ImGui::Separator();

        if (info != nullptr) {
            ImGui::Text("id:         %d", gd::player.id);
            ImGui::Text("level:      %d", info->level);
            ImGui::Text("gold:       %d", info->money);
            ImGui::Text("exp:        %d", info->exp);
            ImGui::Text("explvl:     %d", info->exp_to_level);
            ImGui::Separator();
        }

        if (stats != nullptr) {
            ImGui::Text("dmg:       %.2f", stats->damage_modifier);
            ImGui::Text("a-spd:     %.2f", stats->attack_speed_modifier);
            ImGui::Text("m-spd:     %.2f", stats->move_speed_modifier);
            ImGui::Text("d-rng:     %.2f", stats->dash_speed_modifier);
            ImGui::Text("pu-rng:    %.2f", stats->pickup_range_modifier);
            ImGui::Separator();
        }


        // add stats
        ImVec2 bsize = ImVec2{100, 30};
        if (info != nullptr) {
            if (ImGui::Button("+exp", bsize)) {
                info->exp += data::cnst::EXP_VALUE;
            }
            ImGui::SameLine();
            if (ImGui::Button("+mny", bsize)) {
                info->money += data::cnst::MONEY_VALUE;
            }
        }

        if (stats != nullptr) {
            if (ImGui::Button("+dmg", bsize)) {
                stats->damage_modifier += data::cnst::DAMAGE_POWERUP_VALUE;
            }
            ImGui::SameLine();
            if (ImGui::Button("-dmg", bsize)) {
                stats->damage_modifier -= data::cnst::DAMAGE_POWERUP_VALUE;
            }

            if (ImGui::Button("+atkspd", bsize)) {
                stats->attack_speed_modifier += data::cnst::ATTACK_SPEED_POWERUP_VALUE;
            }
            ImGui::SameLine();
            if (ImGui::Button("-atkspd", bsize)) {
                stats->attack_speed_modifier -= data::cnst::ATTACK_SPEED_POWERUP_VALUE;
            }

            if (ImGui::Button("+mspd", bsize)) {
                stats->move_speed_modifier += data::cnst::MOVE_SPEED_POWERUP_VALUE;
            }
            ImGui::SameLine();
            if (ImGui::Button("-mspd", bsize)) {
                stats->move_speed_modifier -= data::cnst::MOVE_SPEED_POWERUP_VALUE;
            }

            if (ImGui::Button("+prng", bsize)) {
                stats->pickup_range_modifier += data::cnst::PICKUP_RANGE_POWERUP_VALUE;
            }
            ImGui::SameLine();
            if (ImGui::Button("-prng", bsize)) {
                stats->pickup_range_modifier -= data::cnst::PICKUP_RANGE_POWERUP_VALUE;
            }

            if (ImGui::Button("+dash", bsize)) {
                stats->dash_speed_modifier += data::cnst::DASH_DISTANCE_POWERUP_VALUE;
            }
            ImGui::SameLine();
            if (ImGui::Button("-dash", bsize)) {
                stats->dash_speed_modifier -= data::cnst::DASH_DISTANCE_POWERUP_VALUE;
            }
        }
    }

    void DrawWeapsTab(strg::Registry& world) {
        ImVec2 size = ImVec2{100, 30};
        if (ImGui::Button("+pistol", size)) {
            spwn::weapon::EquipPistol(
                world, 
                gd::player.id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("-pistol", size)) {
            spwn::weapon::DequipPistol(world, gd::player.id);
        }

        if (ImGui::Button("+shotgun", size)) {
            spwn::weapon::EquipShotgun(
                world, 
                gd::player.id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("-shotgun", size)) {
            spwn::weapon::DequipShotgun(world, gd::player.id);
        }

        if (ImGui::Button("+rifle", size)) {
            spwn::weapon::EquipRifle(
                world, 
                gd::player.id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("-rifle", size)) {
            spwn::weapon::DequipRifle(world, gd::player.id);
        }

        if (ImGui::Button("+smg", size)) {
            spwn::weapon::EquipSMG(
                world, 
                gd::player.id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("-smg", size)) {
            spwn::weapon::DequipSMG(world, gd::player.id);
        }
        
        if (ImGui::Button("+grenade", size)) {
            spwn::weapon::EquipGrenadeLauncher(
                world, 
                gd::player.id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
        }
        ImGui::SameLine();
        if (ImGui::Button("-grenade", size)) {
            spwn::weapon::DequipGrenadeLauncher(world, gd::player.id);
        }
        ImGui::Separator();

        if (auto* wep = world.TryGetComponent<cmpt::Pistol>(gd::player.id)) {
            ImGui::Text("pistol:");
            ImGui::Text("  dmg:         %d", wep->base.damage);
            ImGui::Text("  pen:         %d", wep->base.penetration);
            ImGui::Text("  atk spd:     %.2f", wep->base.cooldown);
            ImGui::Text("  pjk spd:     %.2f", wep->base.projectile_speed);
            ImGui::Text("  kb scale:    %.2f", wep->base.knockback_scale);
            ImGui::Text("  kb dur:      %.2f", wep->base.knockback_duration);
            ImGui::Separator();
        }

        if (auto* wep = world.TryGetComponent<cmpt::Shotgun>(gd::player.id)) {
            ImGui::Text("shotgun:");
            ImGui::Text("  dmg:         %d", wep->base.damage);
            ImGui::Text("  pen:         %d", wep->base.penetration);
            ImGui::Text("  atk spd:     %.2f", wep->base.cooldown);
            ImGui::Text("  pjk spd:     %.2f", wep->base.projectile_speed);
            ImGui::Text("  kb scale:    %.2f", wep->base.knockback_scale);
            ImGui::Text("  kb dur:      %.2f", wep->base.knockback_duration);
            ImGui::Text("  sprd:        %.2f", wep->spread_deg);
            ImGui::Text("  pcnt:        %d", wep->pellet_count);
            ImGui::Separator();
        }

        if (auto* wep = world.TryGetComponent<cmpt::Rifle>(gd::player.id)) {
            ImGui::Text("rifle:");
            ImGui::Text("  dmg:         %d", wep->base.damage);
            ImGui::Text("  pen:         %d", wep->base.penetration);
            ImGui::Text("  atk spd:     %.2f", wep->base.cooldown);
            ImGui::Text("  pjk spd:     %.2f", wep->base.projectile_speed);
            ImGui::Text("  kb scale:    %.2f", wep->base.knockback_scale);
            ImGui::Text("  kb dur:      %.2f", wep->base.knockback_duration);
            ImGui::Text("  burst cnt:   %d", wep->burst_count);
            ImGui::Text("  burst spd:   %.2f", wep->burst_cooldown);
            ImGui::Separator();
        }

        if (auto* wep = world.TryGetComponent<cmpt::SMG>(gd::player.id)) {
            ImGui::Text("smg:");
            ImGui::Text("  dmg:         %d", wep->base.damage);
            ImGui::Text("  pen:         %d", wep->base.penetration);
            ImGui::Text("  atk spd:     %.2f", wep->base.cooldown);
            ImGui::Text("  pjk spd:     %.2f", wep->base.projectile_speed);
            ImGui::Text("  kb scale:    %.2f", wep->base.knockback_scale);
            ImGui::Text("  kb dur:      %.2f", wep->base.knockback_duration);
            ImGui::Separator();
        }

        if (auto* wep = world.TryGetComponent<cmpt::GrenadeLauncher>(gd::player.id)) {
            ImGui::Text("grenade:");
            ImGui::Text("  dmg:         %d", wep->base.damage);
            ImGui::Text("  pen:         %d", wep->base.penetration);
            ImGui::Text("  atk spd:     %.2f", wep->base.cooldown);
            ImGui::Text("  pjk spd:     %.2f", wep->base.projectile_speed);
            ImGui::Text("  kb scale:    %.2f", wep->base.knockback_scale);
            ImGui::Text("  kb dur:      %.2f", wep->base.knockback_duration);
            ImGui::Text("  ex-dmg:      %d", wep->explosion_damage);
            ImGui::Text("  ex-dur:      %.2f", wep->explosion_duration);
            ImGui::Text("  ex-kb scale: %.2f", wep->explosion_knockback_scale);
            ImGui::Text("  ex-kb dur:   %.2f", wep->explosion_knockback_duration);
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
        ImGui::Checkbox(" god mode", &gd::cheats.god_mode);
        ImGui::Checkbox(" dont spawn enemies", &gd::cheats.dont_spawn_enemies);
        ImGui::Checkbox(" always loot", &gd::cheats.always_drop_loot);
        ImGui::Checkbox(" no loot", &gd::cheats.never_drop_loot);
        ImGui::Separator();

        ImGui::Text("difficulty slider");
        if (ImGui::SliderInt("##diff_slider", &gd::game.difficulty, 0, 2000)) {}
        ImGui::Separator();

        // spawn enemies
        if (ImGui::Button("destroy all enemies")) {
            for (auto entity : world.View<tag::Enemy>()) {
                world.AddComponent<tag::Destroy>(entity);
            }
        }

        if (ImGui::Button("single shooter")) {
            spwn::enemy::GruntWithAPistol(world, Vector3{ 0.0f, 0.0f, 0.0f }, cmpt::AIMoveMode::Random, 500);
        }

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
        ImGui::Separator();

        if (ImGui::Button("wipe loot")) {
            for (auto entity : world.View<cmpt::Loot>()) {
                world.AddComponent<tag::Destroy>(entity);
            }
        }
    }

    void DrawTilemapTab() {
        // tilemap testing
        if (ImGui::Button("draw tilemap")) {
            gd::terrain.DrawTileMap();
        }
    }

    void DrawProfilerTab() {
        #ifdef PROFILER_ENABLED
        // Profiler Tab
        if (ImGui::BeginTabItem("profiler")) {
            if (ImGui::Checkbox("enable profiler: ", &gd::cheats.profiler_enabled)) {
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