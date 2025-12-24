#pragma once

#include <cinttypes>

#include "raylib.h"
#include "data/entity.h"
#include "data/loot.h"

namespace tag {
    struct Player {};
    struct Projectile {};
    struct Enemy {};
    struct Destroy {};
    struct DestroyOnTerrainCollision {};
    struct Terrain {};
    struct Event {};
    struct CameraFollow {};
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
    };

    // non-player characters have this value for AI movement
    struct AIMoveIntent { 
        AIMoveMode mode;
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

    ////////////////////////////////////////////////
    // ANIMATION-ISH
    ////////////////////////////////////////////////
    // rotates in place
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
        double start_time;
        double countdown;
    };

    struct FreezeTime {
        float countdown;
    };

    ////////////////////////////////////////////////
    // ENTITY STATS
    ////////////////////////////////////////////////
    struct Health {
        int max;
        int amount;
    };

    struct Speed {
        float speed;
        float speed_multiplier;
        float dash_multiplier;
    };

    struct DamageDealer {
        int amount;
        int penetration;
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
        float multiplier;
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
    struct Weapon {
        Entity parent;
        data::loot::WeaponKind kind;
    };

    struct WeaponBaseStats { 
        Entity parent;
        data::loot::WeaponKind kind;
        float cooldown;
        float countdown;
        float projectile_speed;
        int damage;
    };

    struct Pistol {
        WeaponBaseStats base_stats;
    };

    struct Shotgun {
        WeaponBaseStats base_stats;
        float spread_deg;
        int pellet_count;
    };

    struct Sniper {
        WeaponBaseStats base_stats;
        int penetration;
    };

    struct Grenade {
        WeaponBaseStats base_stats;
    };

    // struct WeaponStats {
    //     Entity parent;
    //     data::loot::WeaponKind kind;
    //     float cooldown;
    //     float countdown;
    //     float projectile_speed;
    //     int damage;
    // };

    ////////////////////////////////////////////////
    // PROJECTILE SPECIFIC
    ////////////////////////////////////////////////
    struct Penetration {
        int amount;
    };

    struct ExplodeOnImpact {
        float radius;
        float duration;
    };

    ////////////////////////////////////////////////
    // EVENTS
    ////////////////////////////////////////////////
    struct Notification {
        int notification_index;
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
        Color color;
        Model* model;
    };
}