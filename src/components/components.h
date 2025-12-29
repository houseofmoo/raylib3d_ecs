#pragma once

#include <cinttypes>
#include <array>
#include <string_view>
#include "raylib.h"
#include "data/entity.h"
#include "assets/assets.h"

namespace cmpttest {
    struct DamageZone{};
}

namespace tag {
    // struct Player {};
    struct Enemy {};
    struct Event {};
    struct CameraFollow {};    
    struct Destroy {};
    struct DestroyOnTerrainCollision {};
}

namespace cmpt {
    ////////////////////////////////////////////////
    // POSITIONAL
    ////////////////////////////////////////////////
    struct Transform {
        Vector3 position;
        Quaternion rotation;
    };

    ////////////////////////////////////////////////
    // MOVEMENT
    ////////////////////////////////////////////////
    // normal horizontal movement
    typedef Vector3 Velocity; 

    // things that move in an arch (grenades, other throwables ect)
    struct ArchMove { 
        Vector3 start;
        Vector3 end;
        float duration;
        float elapsed;
        float height;
    };

    ////////////////////////////////////////////////
    // COLLISION
    ////////////////////////////////////////////////
    struct Collider {
        Layer layer;
        Mask mask;
        Vector3 offset;
        Vector3 size;
    };

    ////////////////////////////////////////////////
    // INPUT
    ////////////////////////////////////////////////
    enum class AIMoveMode {
        Melee,  // heads directly at player
        Ranged, // stays some min/max distance near player
        Lazy,   // moves generally towards player
        Random, // move entirely randomly
        None,   // stand still
    };

    // non-player characters have this value for AI movement
    struct AIMoveIntent { 
        AIMoveMode mode;
        Vector3 position;
        Vector3 direction;

        Quaternion start_rotation;
        bool rotation_complete;
        float rotation_duration;
        float rotation_elapsed;

        bool stuck;

        void SetSmoothRotation(const Quaternion start_rot) {
            start_rotation = start_rot;
            rotation_complete = false;
            rotation_elapsed = 0.0f;
        }
    };

    struct MeleeMovement {
        Vector3 detour_direction;
        float detour_countdown;
    };

    struct RandomMovement {
        float countdown;
    };

    struct LazyMovement {
        float countdown;
    };

    struct RangedMovement {
        float distance_min;
        float distance_max;
    };

    struct Input {
        Vector3 direction;
        Vector3 mouse_world_position;
    };

    struct AttackIntent {
        bool active;
        Vector3 from_position;
        Vector3 to_position;
        Vector3 direction;
    };

    ////////////////////////////////////////////////
    // ANIMATION-ISH
    ////////////////////////////////////////////////
    struct RotateInPlace { 
        float speed;
    };

    struct DamageFlash {
        float duration;
    };

    struct SpawnAnimation {
        Vector3 start_position;
        Vector3 end_position;
    };

    struct DeathAnimation {
        float duration;
    };

    ////////////////////////////////////////////////
    // SPECIAL CASE
    ////////////////////////////////////////////////
    struct Lifetime {
        float countdown;
    };

    struct FreezeTime {
        float countdown;
    };

    ////////////////////////////////////////////////
    // ENTITY STATS
    ////////////////////////////////////////////////
    struct Player {
        // player only stats
        int level;
        int exp;
        int exp_to_level;
        int money;
        int enemies_defeated;
    };

    struct Stats {
        int current_hp;
        int max_hp;
        float move_speed;

        // modifiers
        float damage_modifier;
        float attack_speed_modifier;
        float move_speed_modifier;
        float dash_speed_modifier;
        float pickup_range_modifier;
    };

    struct DamageDealer {
        int amount;
    };

    struct DamageReceiver {
        int total;
    };

    ////////////////////////////////////////////////
    // STATUS EFFECTS
    ////////////////////////////////////////////////
    struct Invulnerable {
        Mask mask;
        float countdown;
    };

    struct Dash {
        float countdown;
    };

    struct DashExhausted { // not allowed to dash until some time
        float countdown;
    };

    struct AppliesKnockback {
        float scale;
        float duration;
    };

    struct Knockback {
        Vector3 direction;
        float countdown;
    };

    ////////////////////////////////////////////////
    // LOOT
    ////////////////////////////////////////////////
    struct DropsLoot {
        float loot_chance;
    };

    struct Loot {
        data::loot::LootKind kind;
    };

    struct ExpLoot {
        int amount;
    };

    struct MoneyLoot {
        int amount;
    };

    struct PowerupLoot {
        data::loot::PowerupKind kind;
    };

    struct WeaponLoot {
        data::loot::WeaponKind kind;
    };
    
    ////////////////////////////////////////////////
    // WEAPONS
    ////////////////////////////////////////////////
    struct EquippedWeapons {
        std::array<data::loot::WeaponKind, 4> weapons;
    };

    struct WeaponBase { 
        Entity parent;
        data::loot::WeaponKind kind;
        float cooldown;
        float countdown;
        int damage;
        int penetration;
        float projectile_speed;
        float knockback_scale;
        float knockback_duration;
        Layer layer;      // layer projectiles will be spawned on
        Layer mask;       // layer those projectiles interact with
        asset::SoundFxType soundfx_type;
    };

    struct Pistol {
        WeaponBase base;
    };

    struct Shotgun {
        WeaponBase base;
        float spread_deg;
        int pellet_count;
    };

    struct Rifle {
        WeaponBase base;
        int burst_count;
        int burst_completed;
        float burst_cooldown;
        float burst_countdown;
    };

    struct Sniper {
        WeaponBase base;
    };

    struct RailGun {
        WeaponBase base;
    };

    struct SMG {
        WeaponBase base;
    };

    struct GrenadeLauncher {
        WeaponBase base;
        
        float arch_duration;
        float arch_max_height;

        int explosion_damage;
        Vector3 explosion_start_size;
        Vector3 explosion_end_size;
        float explosion_duration;
        float explosion_knockback_scale;
        float explosion_knockback_duration;
    };

    struct RocketLauncher {
        WeaponBase base_stats;
        int explosion_damage;
        Vector3 explosion_start_size;
        Vector3 explosion_end_size;
        float explosion_duration;
        float explosion_knockback_scale;
        float explosion_knockback_duration;
    };

    ////////////////////////////////////////////////
    // ATTACKa SPECIFIC
    ////////////////////////////////////////////////
    struct Projectile {
        int penetration;
    };

    struct ExplodeOnDestroy {
        Vector3 start_size;
        Vector3 end_size;
        int damage;
        float duration;
        float knockback_scale;
        float knockback_duration;
        Layer layer;      // layer projectiles will be spawned on
        Layer mask;       // layer those projectiles interact with
    };

    struct Explosion {
        float duration;
        float elapsed;
        Vector3 start_size;
        Vector3 end_size;
    };

    struct LargeAoE {}; // indicates this hurt box is larger than normal

    ////////////////////////////////////////////////
    // EVENTS
    ////////////////////////////////////////////////
    struct Notification {
        std::string_view msg;
    };

    struct LootEvent {
        Entity id;
        data::loot::LootKind kind;
    };

    ////////////////////////////////////////////////
    // DRAW/MODELS
    ////////////////////////////////////////////////
    struct Draw {
        Vector3 size;
        Vector3 scale;
        Color color;
        Model* model;
    };
}