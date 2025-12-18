#include "systems/systems.h"

// #include <iostream>
// #include <cmath>
// #include <string>
#include <format>

#include "rlgl.h"

#include "data/player/player.h"
#include "data/game/game.h"
#include "resources/assets.h"

#include "spawners/camera/camera.h"
#include "spawners/enemy/enemy.h"
#include "spawners/map/map.h"
#include "spawners/player/player.h"
#include "spawners/weapon/pistol.h"

#include "systems/ai/ai.h"
#include "systems/camera/camera.h"
#include "systems/collision/entity_collision_system.h"
#include "systems/collision/collision_handlers.h"
#include "systems/damage_system.h"
#include "systems/enemy/timed_spawn.h"
#include "systems/input_system.h"
#include "systems/movement_system.h"
#include "systems/terrain_collision.h"
#include "systems/velocity_system.h"


#include "systems/attack_system.h"
#include "systems/drop_loot.h"
#include "systems/animation_system.h"
#include "systems/player/player_levelup.h"
#include "systems/cleanup_system.h"
#include "systems/status_effects_system.h"
#include "systems/constraint.h"
#include "systems/notification/notification.h"


#include "utils/print.h"

namespace sys {
    Camera3D camera;
    Storage::Registry world;
    BoundingBox boundary;

    void InitWorld() noexcept {
        camera = spwn::camera::Camera();
        boundary = {
            Vector3{-60.0f, 0.0f, -30.0f},
            Vector3{60.0f, 55.0f, 30.0f}
        };

        spwn::map::GenerateMap(world, boundary);
        data::player::g_player.id = spwn::player::Player(world);
        spwn::weapon::Pistol(world, data::player::g_player.id);
    }

    void RunUpdateSystems(const float delta_time) noexcept {
        if (data::game::g_paused) {
            return;
        }

        
        // weapons as loot?
        // if so, make them long rectanlges
        // do we allow the player to hold as many weapons or a set amount?
        // we want the enemies to have weapons at some point as well so they can shoot at the player

        // TODO: disable colliders via masks
        // as in when the player is invul, change their mask to no longer include anything but terrain and pickup

        // TODO: re-add loot upgrades, we can pick things up but they do nothing
        // also re-add the sound for pickups

        // spawn additional enemies
        sys::enemy::SpawnEnemyInterval(world, boundary, delta_time);
        
        // input/move ai move intent
        sys::input::PlayerInput(world, camera);
        sys::input::AIMoveIntent(world);

        // apply movement to velocity
        sys::se::ApplyStatusEffects(world);
        sys::mov::ApplyDash(world, delta_time);
        sys::mov::ApplyPlayerMovement(world, delta_time);
        sys::mov::ApplyAIMovement(world, delta_time);
        sys::cam::CameraMovement(world, camera, delta_time);

        // apply velocity to position
        sys::vel::ApplyVelocity(world, delta_time);
        sys::vel::ApplyArch(world, delta_time);
        sys::vel::ApplyRotateInPlace(world, delta_time);
        
        // sys::loot::LootMovement(world, delta_time);
        // sys::player::PlayerMovement(world, delta_time);
        // sys::enemy::EnemyMovement(world, delta_time);
        // sys::proj::BulletMovement(world, delta_time);
        // sys::proj::GrenadeMovement(world, delta_time);
        // sys::cam::CameraMovement(world, camera, delta_time);
        //sys::ApplyVelocity(world, delta_time);

        sys::atk::WeaponAttacks(world, delta_time, rsrc::asset::shoot_fx); // shoot_fx
        // sys::loot::LootPlayerCollision(world, rsrc::asset::powerup_fx); // powerup_fx
        // sys::proj::BulletEnemyCollision(world, rsrc::asset::damage_fx); // damage_fx
        // sys::enemy::EnemyPlayerCollision(world);
        //sys::col::TerrainCollision(world);
        sys::col::EntityCollision(world);
        sys::col::DamageOnCollision(world);
        sys::col::DestroyOnCollision(world);
        sys::col::KnockbackOnCollision(world);
        sys::col::PickupOnCollision(world);
        sys::col::TriggerOnCollision(world);
        sys::col::RepositionOnCollision(world);
        sys::dmg::ApplyDamage(world);
        sys::se::RemoveStatsusEffects(world);

        // sys::enemy::EnemyTerrainCollision(world);
        // sys::proj::BulletTerrainCollision(world);
        // sys::player::PlayerTerrainCollision(world);

        sys::player::PlayerLevelup(world);
        sys::SpawnAnimation(world);
        sys::DeathAnimation(world, delta_time);
        sys::loot::LootDrop(world);
        sys::cleanup::Cleanup(world, delta_time);
        sys::ConstraintToWorld(world, boundary);
    }

    void RunEntityDrawSystems(const float delta_time) noexcept {
        // draw all drawable entities
        int entities_count = 0;

        for (auto entity : sys::world.View<cmpt::Draw, cmpt::Transform>()) {
            auto& draw = sys::world.GetComponent<cmpt::Draw>(entity);
            auto& trans = sys::world.GetComponent<cmpt::Transform>(entity);

            entities_count += 1;
            if (draw.model == nullptr) { // terrain does not have a model atm
                DrawCubeV(trans.position, draw.size, draw.color);
                DrawCubeWiresV(trans.position, draw.size, BLACK);
            } else {
                Color color = draw.color;
                if (sys::world.HasComponent<cmpt::DamageFlash>(entity)) {
                    auto& flash = sys::world.GetComponent<cmpt::DamageFlash>(entity);
                    flash.duration -= delta_time;
                    if (flash.duration < 0.0f) {
                        sys::world.RemoveComponent<cmpt::DamageFlash>(entity);
                    }
                    color = WHITE;
                }

                Vector3 axis;
                float angle = 0.0f;
                QuaternionToAxisAngle(trans.rotation, &axis, &angle);
                axis = Vector3Normalize(axis);

                DrawModelEx(*draw.model, trans.position, axis, RAD2DEG * angle, {1,1,1}, color);
                rlPushMatrix();
                    rlTranslatef(trans.position.x, trans.position.y, trans.position.z);   // pivot at model position
                    rlRotatef(RAD2DEG * angle, axis.x, axis.y, axis.z);                   // apply same rotation
                    DrawCubeWiresV({0,0,0}, draw.size, BLACK);                            // draw in local space
                rlPopMatrix();
            }
        }
        
        // ground
        float width = sys::boundary.max.x - sys::boundary.min.x;
        float length = sys::boundary.max.z - sys::boundary.min.z; 
        DrawCube(Vector3{0.0f, -0.51f, 0.0f}, width, 1.0f, length, GRAY);

        DrawGrid(60, 1.0f);
        data::game::g_number_entities = entities_count;
    }

    void RunUIDrawSystems(const float delta_time) noexcept {
        int player_hp = 0;
        if (sys::world.HasComponent<cmpt::Health>(data::player::g_player.id)) {
            auto& hp_comp = sys::world.GetComponent<cmpt::Health>(data::player::g_player.id);
            player_hp = hp_comp.amount;
        }
        
        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 30, 20, 20, RAYWHITE);
        DrawText(std::format("Entities Drawn: {}", data::game::g_number_entities).c_str(), 30, 40, 20, RAYWHITE);
        DrawText(std::format("HP: {}/100", player_hp).c_str(), 30, 80, 20, RAYWHITE);
        DrawText(std::format("Level: {}", data::player::g_player.level).c_str(), 30, 100, 20, RAYWHITE);
        DrawText(std::format("Difficulty : {}", data::game::g_difficulty_level).c_str(), 30, 140, 20, RAYWHITE);
        DrawText(std::format("Enemies Defeated: {}", data::player::g_player.enemies_defeated).c_str(), 30, 160, 20, RAYWHITE);
        sys::noti::DrawNotifications(world, delta_time);
    }
}