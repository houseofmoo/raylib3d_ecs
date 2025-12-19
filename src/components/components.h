#pragma once

#include <cinttypes>

#include "raylib.h"
#include "data/entity.h"
#include "data/loot.h"

namespace tag {
    struct Player {};
    struct Weapon {};
    struct Projectile {};
    struct Enemy {};
    struct Destroy {};
    struct Terrain {};
    struct DropsLoot {};
    struct Event {};
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
    
    struct DestroyOnContact {
        int penetration;
    };

    ////////////////////////////////////////////////
    // INPUT
    ////////////////////////////////////////////////
    enum class MoveIntentType {
        Melee,
        Ranged,
        Random,
    };

    // non-player characters have this value for AI movement
    struct MoveIntent { 
        MoveIntentType type;
        Vector3 direction;
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

    ////////////////////////////////////////////////
    // ENTITY STATS
    ////////////////////////////////////////////////
    struct Health {
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

    struct Knockback {
        Vector3 direction;
        float countdown;
    };

    ////////////////////////////////////////////////
    // LOOT/WEAPONS
    ////////////////////////////////////////////////
    struct Loot {
        data::loot::LootKind kind;
    };

    struct WeaponStats {
        Entity parent;
        data::loot::WeaponKind kind;
        float cooldown;
        float countdown;
        float projectile_speed;
        int damage;
    };

    struct Spread {
        int pellet_count;
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