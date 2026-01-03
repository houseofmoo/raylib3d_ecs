#include "systems/systems.h"

#include <format>
#include "rlgl.h"

#include "data/global_data.h"
#include "assets/asset_loader.h"
#include "assets/assets.h"
#include "utils/debug.h"

#include "spawners/system/map/map.h"
#include "spawners/equip/weapon/weapons.h"
#include "spawners/world/player/player.h"

#include "systems/animation_system.h"
#include "systems/attack/attack_system.h"
#include "systems/camera/camera.h"
#include "systems/cleanup/cleanup.h"
#include "systems/collisions/entity_collision_system.h"
#include "systems/collisions/collision_handlers.h"
#include "systems/damage_system.h"
#include "systems/loot/drop_loot.h"
#include "systems/levelup_system.h"
#include "systems/spawn_enemies/spawn_enemies.h"
#include "systems/events/loot_pickedup_event_system.h"
#include "systems/events/notification_event_system.h"
#include "systems/input/input_system.h"
#include "systems/movement_system.h"
#include "systems/status_effects_system.h"
#include "systems/velocity_system.h"

#include "spawners/damage_zone.h"

namespace sys {
    strg::Registry world;
    constinit Camera3D camera = {
        .position = data::cnst::CAMERA_START_POSITION,
        .target = data::cnst::CAMERA_TARGET,
        .up = data::cnst::CAMERA_UP,
        .fovy = data::cnst::CAMERA_FOVY,
        .projection = CAMERA_PERSPECTIVE
    };

    void StartGame() {
        world.Reset();

        gd::game = {
            .difficulty = 0,
            .enemy_spawn_countdown = 0.0f,
            .enemy_spawn_interval = data::cnst::MAX_SPAWN_INTERVAL,
            .enemy_count = 0,
            .entity_count = 0
        };

        spwn::map::GenerateMap(world);
        gd::player.id = spwn::player::Player(world);
        spwn::weapon::EquipPistol(
            world, 
            gd::player.id, 
            data::cnst::PLAYER_PROJECTILE_LAYER, 
            data::cnst::PLAYER_PROJECTILE_LAYER_MASK
        );
    }

    void RunGameSystems(state::NavRequest& nav, const float delta_time) {
        if (world.HasComponent<cmpt::FreezeTime>(gd::player.id)) {
            auto& ft = world.GetComponent<cmpt::FreezeTime>(gd::player.id);
            ft.countdown -= delta_time;
            if (ft.countdown <= 0.0f) {
                world.RemoveComponent<cmpt::FreezeTime>(gd::player.id);
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

        sys::atk::WeaponAttacks(world, delta_time);
        
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
        
        sys::loot::DropLoot(world);
        sys::evt::HandleLootPickedupEvents(world, nav);

        sys::vel::ApplyVelocity(world, delta_time);
        sys::vel::ApplyArch(world, delta_time);
        sys::vel::ApplyRotateInPlace(world, delta_time);

        sys::lvl::LevelupSystem(world, nav);

        sys::cu::LifetimeTimer(world, delta_time);
        sys::cu::OnDestroyEffects(world);
        sys::cu::Destroy(world, nav);
    }
}