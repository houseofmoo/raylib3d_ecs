#include "systems/systems.h"

#include <format>
#include "rlgl.h"
#include "raygui.h"

#include "data/game/game.h"
#include "resources/asset_loader.h"
#include "utils/debug.h"

#include "spawners/system/camera/camera.h"
#include "spawners/system/map/map.h"
#include "spawners/equip/weapon/weapons.h"
#include "spawners/world/player/player.h"

#include "systems/animation_system.h"
#include "systems/attack/attack_system.h"
#include "systems/camera/camera.h"
#include "systems/cleanup_system.h"
#include "systems/collisions/entity_collision_system.h"
#include "systems/collisions/collision_handlers.h"
#include "systems/damage_system.h"
#include "systems/drop_loot.h"
#include "systems/timed_spawn_system.h"
#include "systems/events/loot_pickedup_event_system.h"
#include "systems/events/notification_event_system.h"
#include "systems/input/input_system.h"
#include "systems/movement_system.h"
#include "systems/status_effects_system.h"
#include "systems/velocity_system.h"

#include "spawners/damage_zone.h"

namespace sys {
    Camera3D camera;
    strg::Registry world;

    void InitWorld() {
        camera = spwn::camera::Camera();
     }

    void StartGame() {
        world.Reset();
        data::g_game.Reset();
        spwn::map::GenerateMap(world);
        data::g_player_id = spwn::player::Player(world);
        spwn::weapon::EquipPistol(
            world, 
            data::g_player_id, 
            data::cnst::PLAYER_PROJECTILE_LAYER, 
            data::cnst::PLAYER_PROJECTILE_LAYER_MASK
        );
    }

    void RunUpdateSystems(const float delta_time) {
        // TODO: temp for crate menu testing
        if (data::g_weapon_select_menu.show) {
            return;
        }

        // TODO: re-add the sound for pickups
        if (world.HasComponent<cmpt::FreezeTime>(data::g_player_id)) {
            auto& ft = world.GetComponent<cmpt::FreezeTime>(data::g_player_id);
            ft.countdown -= delta_time;
            if (ft.countdown <= 0.0f) {
                world.RemoveComponent<cmpt::FreezeTime>(data::g_player_id);
            }
            sys::cam::CameraMovement(world, camera, delta_time, true);
            return;
        }

        sys::SpawnEnemyInterval(world, delta_time);

        sys::input::PlayerInput(world, camera);
        sys::input::AIMoveIntent(world, delta_time);
        sys::input::AttackIntent(world);

        sys::se::ApplyStatusEffects(world);
        sys::mov::ApplyPlayerMovement(world);
        sys::mov::ApplyAIMovement(world, delta_time);
        sys::cam::CameraMovement(world, camera, delta_time, false);

        sys::atk::WeaponAttacks(world, delta_time, rsrc::asset::shoot_fx); // shoot_fx
        
        sys::col::EntityCollision(world);
        sys::col::DamageOnCollision(world);
        sys::col::DestroyOnCollision(world);
        sys::col::KnockbackOnCollision(world);
        sys::col::PickupOnCollision(world);
        sys::col::TriggerOnCollision(world);
        sys::col::RepositionOnCollision(world);

        sys::dmg::ApplyDamage(world);
        sys::se::RemoveStatsusEffects(world, delta_time);
        
        sys::SpawnAnimation(world);
        sys::DeathAnimation(world);
        
        sys::loot::LootDrop(world);
        sys::evt::HandleLootPickedupEvents(world);

        sys::vel::ApplyVelocity(world, delta_time);
        sys::vel::ApplyArch(world, delta_time);
        sys::vel::ApplyRotateInPlace(world, delta_time);

        sys::cleanup::Cleanup(world, delta_time);
        sys::cleanup::OnDestroy(world);
        sys::cleanup::Destroy(world);
    }

    void RunEntityDrawSystems(const float delta_time) {
        PROFILE_SCOPE("RunEntityDrawSystems()");
        // draw all drawable entities
        int entities_count = 0;
        int enemies_count = 0;

        for (auto entity : sys::world.View<cmpt::Draw, cmpt::Transform>()) {
            auto& draw = sys::world.GetComponent<cmpt::Draw>(entity);
            auto& trans = sys::world.GetComponent<cmpt::Transform>(entity);

            entities_count += 1;
            if (draw.model == nullptr) { // terrain does not have a model atm
                DrawCubeV(trans.position, draw.size, draw.color);
                DrawCubeWiresV(trans.position, draw.size, BLACK);
            } else {
                if (world.HasComponent<tag::Enemy>(entity)) enemies_count += 1;

                Color color = draw.color;
                if (world.HasComponent<cmpt::DamageFlash>(entity)) {
                    auto& flash = sys::world.GetComponent<cmpt::DamageFlash>(entity);
                    flash.duration -= delta_time;
                    if (flash.duration < 0.0f) {
                        world.RemoveComponent<cmpt::DamageFlash>(entity);
                    }
                    color = WHITE;
                }
                if (world.HasComponent<cmpt::Invulnerable>(entity)) {
                    color.a = 128;
                }

                Vector3 axis;
                float angle = 0.0f;
                QuaternionToAxisAngle(trans.rotation, &axis, &angle);
                axis = Vector3Normalize(axis);

                DrawModelEx(*draw.model, trans.position, axis, RAD2DEG * angle, draw.scale, color);
                rlPushMatrix();
                    rlTranslatef(trans.position.x, trans.position.y, trans.position.z);   // pivot at model position
                    rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);                   // apply same rotation
                    DrawCubeWiresV({0,0,0}, draw.size, BLACK);                            // draw in local space
                rlPopMatrix();
            }
        }
        
        // ground
        float width = data::cnst::PLAY_AREA.max.x - data::cnst::PLAY_AREA.min.x;
        float length = data::cnst::PLAY_AREA.max.z - data::cnst::PLAY_AREA.min.z; 
        DrawCube(Vector3{0.0f, -0.51f, 0.0f}, width, 1.0f, length, GRAY);
        for (int x = (int)data::cnst::PLAY_AREA.min.x; x < data::cnst::PLAY_AREA.max.x; x+=2) {
            Vector3 top =    { (float)x, 0.0f, data::cnst::PLAY_AREA.min.z };
            Vector3 bottom = { (float)x, 0.0f, data::cnst::PLAY_AREA.max.z };
            DrawLine3D(top, bottom, BLACK);
        }
        for (int z = (int)data::cnst::PLAY_AREA.min.z; z < data::cnst::PLAY_AREA.max.z; z+=2) {
            Vector3 top =    { data::cnst::PLAY_AREA.min.x, 0.0f, (float)z };
            Vector3 bottom = { data::cnst::PLAY_AREA.max.x, 0.0f, (float)z };
            DrawLine3D(top, bottom, BLACK);
        }

        // draw play area walls
        Vector3 position = { data::cnst::PLAY_AREA.min.x - 0.5f, 3.0f, 0.0f };
        Vector3 size = { 1.0f, 6.0f, data::cnst::PLAY_AREA.max.z - data::cnst::PLAY_AREA.min.z };
        DrawCubeV(position, size, DARKGRAY);
        DrawCubeWiresV(position, size, BLACK);

        position = { data::cnst::PLAY_AREA.max.x + 0.5f, 3.0f, 0.0f };
        size = { 1.0f, 6.0f, data::cnst::PLAY_AREA.max.z - data::cnst::PLAY_AREA.min.z };
        DrawCubeV(position, size, DARKGRAY);
        DrawCubeWiresV(position, size, BLACK);

        position = {  0.0f, 3.0f, data::cnst::PLAY_AREA.min.z - 0.5f };
        size = { data::cnst::PLAY_AREA.max.x - data::cnst::PLAY_AREA.min.x, 6.0f, 1.0f };
        DrawCubeV(position, size, DARKGRAY);
        DrawCubeWiresV(position, size, BLACK);

        DrawLine3D(
            Vector3{ data::cnst::PLAY_AREA.min.x + 0.01f, 6.0f, data::cnst::PLAY_AREA.min.z + 0.01f },
            Vector3{ data::cnst::PLAY_AREA.min.x + 0.01f, 0.0f, data::cnst::PLAY_AREA.min.z + 0.01f },
            BLACK
        );
        DrawLine3D(
            Vector3{ data::cnst::PLAY_AREA.max.x - 0.01f, 6.0f, data::cnst::PLAY_AREA.min.z + 0.01f },
            Vector3{ data::cnst::PLAY_AREA.max.x - 0.01f, 0.0f, data::cnst::PLAY_AREA.min.z + 0.01f },
            BLACK
        );

        data::g_game.entity_count = entities_count;
        data::g_game.enemy_count = enemies_count;
    }

    void RunUIDrawSystems(const float delta_time) {
        PROFILE_SCOPE("RunUIDrawSystems()");
        auto& player_stats = world.GetComponent<cmpt::Stats>(data::g_player_id);
        auto& player_info = world.GetComponent<cmpt::Player>(data::g_player_id);
        
        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 30, 20, 20, RAYWHITE);
        DrawText(std::format("Entities Drawn: {}", data::g_game.entity_count).c_str(), 30, 40, 20, RAYWHITE);
        DrawText(std::format("HP: {}/{}", player_stats.current_hp, player_stats.max_hp).c_str(), 30, 80, 20, RAYWHITE);
        DrawText(std::format("Level: {}", player_info.level).c_str(), 30, 100, 20, RAYWHITE);
        DrawText(std::format("Difficulty : {}", data::g_game.difficulty).c_str(), 30, 140, 20, RAYWHITE);
        DrawText(std::format("Enemies Defeated: {}", player_info.enemies_defeated).c_str(), 30, 160, 20, RAYWHITE);
        sys::evt::DrawNotifications(world, delta_time);
    }

    void RunWeaponSelectDrawSystem(const int screen_width, const int screen_height) {
        // darken background
        DrawRectangle(0, 0, screen_width, screen_height, Color{0,0,0,175});

        // display choices
        auto rec = Rectangle{ (screen_width * 0.5f) - 75.0f, 300.0f, 150.0f, 50.0f };
        if (::GuiButton(rec, "Pistol")) {
            spwn::weapon::EquipPistol(
                world, 
                data::g_player_id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
            data::g_weapon_select_menu.show = false;
        }

        rec.y += 50.0f;
        if (::GuiButton(rec, "Shotgun")) {
            spwn::weapon::EquipShotgun(
                world, 
                data::g_player_id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
            data::g_weapon_select_menu.show = false;
        }

        rec.y += 50.0f;
        if (::GuiButton(rec, "Rifle")) {
            spwn::weapon::EquipRifle(
                world, 
                data::g_player_id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
            data::g_weapon_select_menu.show = false;
        }

        rec.y += 50.0f;
        if (::GuiButton(rec, "SMG")) {
            spwn::weapon::EquipSMG(
                world, 
                data::g_player_id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
            data::g_weapon_select_menu.show = false;
        }

        rec.y += 50.0f;
        if (::GuiButton(rec, "Grenade Launcher")) {
            spwn::weapon::EquipGrenadeLauncher(
                world, 
                data::g_player_id, 
                data::cnst::PLAYER_PROJECTILE_LAYER, 
                data::cnst::PLAYER_PROJECTILE_LAYER_MASK
            );
            data::g_weapon_select_menu.show = false;
        }
    }
}