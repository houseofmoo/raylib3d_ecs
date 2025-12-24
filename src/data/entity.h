#pragma once

#include <array>
#include <cinttypes>
#include <string_view>
#include "raylib.h"

using Entity = uint32_t;
using Layer = uint32_t;
using Mask = uint32_t;

namespace data {
    namespace loot {
        enum class LootKind {
            Exp,
            Money,
            Powerup,
            Weapon,
            WeaponCrate,
        };

        enum class PowerupKind {
            NONE,
            Damage,
            AttackSpeed,
            MoveSpeed,
            PickupRange,
            DashDistance,
            Health,
            MaxHp,
            Last
        };

        inline PowerupKind GetRandomPowerupKind() {
            return (PowerupKind)GetRandomValue(
                (int)data::loot::PowerupKind::NONE + 1, 
                (int)data::loot::PowerupKind::Last - 1
            );;
        }

        enum class WeaponKind {
            NONE,
            Pistol,          // single pellet, starter wep expected to be not awesome
            Shotgun,         // spread pattern (3 pellets)
            Rifle,           // richochettes?
            Sniper,          // penetration (3 enemies)
            RailGun,         // high speed + penetration (5 enemies)
            SMG,             // high attack speed
            Grenade,         // arch + area damage
            RocketLauncher,  // slow moving + area damage
            Last
        };

        inline WeaponKind GetRandomWeaponKind() {
            return (WeaponKind)GetRandomValue(
                (int)data::loot::WeaponKind::NONE + 1, 
                (int)data::loot::WeaponKind::Last - 1
            );
        }
    }
    
    // layer masks
    namespace layer {
        constexpr Layer PLAYER     = 1 << 0;
        constexpr Layer ENEMY      = 1 << 1;
        constexpr Layer PROJECTILE = 1 << 2;
        constexpr Layer LOOT       = 1 << 3;
        //constexpr Layer TERRAIN    = 1 << 4;
        //constexpr Layer LAYER_TBD  = 1 << 5;
        //constexpr Layer LAYER_TBD  = 1 << 6;
        //constexpr Layer LAYER_TBD  = 1 << 7;
        //constexpr Layer LAYER_TBD  = 1 << 8;
        //constexpr Layer LAYER_TBD  = 1 << 9;

        inline bool InteractsOneWay(Layer layer_a, Mask mask_a, Layer layer_b, Mask mask_b) {
            // if either interact with the other
            return (layer_a & mask_b) > 0 || (layer_b & mask_a) > 0;
        }

        inline bool InteractsBoth(Layer layer_a, Mask mask_a, Layer layer_b, Mask mask_b) {
            // must interact with each other
            return (layer_a & mask_b) > 0 && (layer_b & mask_a) > 0;
        }
    }

    namespace cnst {
        // camera
        constexpr Vector3 CAMERA_START_POSITION   = { 5.0f, 3.0f, 5.0f };
        constexpr Vector3 CAMERA_TARGET           = { 0.0f, 1.0f, 0.0f };
        constexpr Vector3 CAMERA_UP               = { 0.0f, 1.0f, 0.0f };
        constexpr float   CAMERA_FOVY             = 60.0f;
        constexpr Vector3 CAMERA_OFFSET           = { 0.0f, 35.0f, 15.0f };   // up + back (fixed)
        constexpr Vector3 TARGET_OFFSET           = { 0.0f, 0.5f, 0.0f };     // look a bit above feet
        constexpr float   CAMERA_FOLLOW_SHARPNESS = 14.0f;                   // bigger = tighter
        constexpr float   CAMERA_SHAKE_STR        = 5.0f;
        constexpr float   CAMERA_SHAKE_FREQ       = 30.0f;
        
        // general
        constexpr BoundingBox PLAY_AREA = {
            .min = Vector3{-60.0f, 0.0f, -30.0f},
            .max = Vector3{60.0f, 55.0f, 30.0f}
        };
        constexpr Layer ENEMY_LAYER      = data::layer::ENEMY;
        constexpr Layer ENEMY_LAYER_MASK = data::layer::PLAYER | data::layer::ENEMY | data::layer::PROJECTILE;
        constexpr float IMPACT_FREZE     = 0.1f;
        constexpr float DAMAGE_FLASH     = 0.1f;

        // player
        constexpr Color   PLAYER_COLOR              = Color{ 0, 121, 241, 255 };
        constexpr Vector3 PLAYER_SIZE               = { 1.0f, 2.0f, 1.0f };
        constexpr int     PLAYER_START_HP           = 50;
        constexpr float   PLAYER_SPEED              = 10.0f;
        constexpr Layer   PLAYER_LAYER              = data::layer::PLAYER;
        constexpr Layer   PLAYER_LAYER_MASK         = data::layer::ENEMY | data::layer::LOOT;
        constexpr float   PLAYER_DASH_RANGE         = 3.0f;
        constexpr float   PLAYER_DASH_DURATION      = 0.11f;

        // loot
        constexpr int     LOOT_CHANCE             = 50;
        constexpr int     LOOT_BADLUCK_PROTECTION = 10;
        constexpr float   LOOT_ROTATION_SPEED     = 2.5f;
        constexpr Vector3 MIN_LOOT_COLLIDER       = { 0.5f, 0.5f, 0.5f };
        constexpr float   LOOT_LIFETIME           = 15.0f;
        constexpr Layer   LOOT_LAYER              = data::layer::LOOT;
        constexpr Layer   LOOT_LAYER_MASK         = data::layer::PLAYER;
        constexpr Color   EXP_COLOR               = Color{ 102, 191, 255, 255 }; // SKYBLUE
        constexpr Vector3 EXP_SIZE                = { 0.3f, 0.3f, 0.3f };
        constexpr Vector3 MONEY_SIZE              = { 0.3f, 0.3f, 0.3f };
        constexpr Color   MONEY_COLOR             = Color{ 255, 203, 0, 255 }; // GOLD
        constexpr Vector3 POWERUP_SIZE            = { 0.5f, 0.5f, 0.5f };
        constexpr Vector3 WEAPON_SIZE             = { 1.25f, 0.35f, 0.35f };
        constexpr Vector3 WEAPON_CRATE_SIZE       = { 1.25f, 1.5f, 0.75f };
        constexpr Color   WEAPON_CRATE_COLOR      = Color{ 211, 176, 131, 255 };

        constexpr int   EXP_VALUE                   = 1; 
        constexpr int   MONEY_VALUE                 = 1;
        constexpr float DAMAGE_POWERUP_VALUE        = 0.1f;
        constexpr float ATTACK_SPEED_POWERUP_VALUE  = 0.1f;
        constexpr float MOVE_SPEED_POWERUP_VALUE    = 0.05f;
        constexpr float PICKUP_RANGE_POWERUP_VALUE  = 0.25f;
        constexpr float DASH_DISTANCE_POWERUP_VALUE = 0.1f;
        constexpr int   HP_POWERUP_VALUE            = 5;
        constexpr int   HPMAX_POWERUP_VALUE         = 10;

        // powerup colors
        constexpr std::array<Color, static_cast<int>(data::loot::PowerupKind::Last)> POWERUP_COLORS = {
            BLACK,      // NONE
            GREEN,      // damage
            LIME,       // attack speed
            DARKGREEN,  // move speed
            DARKBLUE,   // pick up range
            PURPLE,     // dash distance
            VIOLET,     // health
            DARKPURPLE  // max hp
        };

        // weapon colors
        constexpr std::array<Color, static_cast<int>(data::loot::WeaponKind::Last)> WEAPON_COLORS = {
            BLACK,      // NONE
            LIGHTGRAY,  // pistol
            GRAY,       // shotgun
            DARKGRAY,   // rifle
            YELLOW,     // sniper
            ORANGE,     // rail gun
            PINK,       // smg
            RED,        // grenade
            MAROON,     // rocket launcher
        };

        // weapons
        constexpr float MIN_WEAPON_COOLDOWN     = 0.1f;

        constexpr int   PISTOL_DAMAGE             = 5;
        constexpr float PISTOL_COOLDOWN           = 0.5f;
        constexpr float PISTOL_PROJECTILE_SPEED   = 25.0f;
        constexpr int   PISTOL_PENETRATION        = 1;
        constexpr float PISTOL_KNOCKBACK_SCALE    = 5.0f;
        constexpr float PISTOL_KNOCKBACK_DURATION = 0.10f;

        constexpr int   SHOTGUN_DAMAGE             = 9;
        constexpr float SHOTGUN_COOLDOWN           = 2.5f;
        constexpr float SHOTGUN_PROJECTILE_SPEED   = 20.0f;
        constexpr int   SHOTGUNL_PENETRATION       = 1;
        constexpr float SHOTGUN_SPREAD             = 30.0f;
        constexpr int   SHOTGUN_PELLET_COUNT       = 3;
        constexpr float SHOTGUN_KNOCKBACK_SCALE    = 10.0f;
        constexpr float SHOTGUN_KNOCKBACK_DURATION = 0.10f;

        constexpr int     GRENADE_DAMAGE           = 25;
        constexpr float   GRENADE_COOLDOWN         = 2.0f;
        constexpr float   GRENADE_PROJECTILE_SPEED = 25.0f;
        constexpr Vector3 GRENADE_SIZE             = { 0.5f, 0.5f, 0.5f };

        // projectiles
        constexpr Color   PROJECTILE_COLOR      = Color{ 230, 41, 55, 255 };
        constexpr Vector3 PROJECTILE_SIZE       = { 0.25f, 0.25f, 0.25f };
        constexpr Layer   PROJECTILE_LAYER      = data::layer::PROJECTILE;
        constexpr Layer   PROJECTILE_LAYER_MASK = data::layer::ENEMY;
        constexpr float   PROJECTILE_LIFETIME   = 10.0f;

        // arch projectiles
        constexpr float ARCH_MAX_HEIGHT = 10.0f;
        constexpr float ARCH_DURATION = 1.0f; // doubles as lifetime since the ground does not destroy projectiles

        // status effects
        constexpr float INVULNRABILITY_CD = 1.0f;

        // enemy general
        constexpr float MAX_SPAWN_INTERVAL         = 2.5f;
        constexpr float MIN_SPAWN_INTERVAL         = 0.5f;
        const float     ENEMY_ROTATION_DURATION    = 0.2f;
        const float     ENEMY_DETOUR_DURATION      = 3.0f;
        const float     ENEMY_LAZY_RETARGET_TIME   = 2.0f;
        const float     ENEMY_RANDOM_RETARGET_TIME = 2.0f;

        // grunt
        constexpr Color   GRUNT_COLOR              = Color{ 255, 0, 255, 255 };
        constexpr Vector3 GRUNT_SIZE               = { 1.0f, 1.5f, 1.0f };
        constexpr int     GRUNT_MIN_HP             = 20;
        const float       GRUNT_SPEED              = 5.0f;
        const int         GRUNT_MELEE_DMG          = 5;
        constexpr float   GRUNT_LOOT_MULTIPLIER    = 1.0f;
        constexpr float   GRUNT_KNOCKBACK_SCALE    = 20.0f;
        constexpr float   GRUNT_KNOCKBACK_DURATION = 0.1f;
        constexpr Layer   GRUNT_LAYER              = ENEMY_LAYER;
        constexpr Layer   GRUNT_LAYER_MASK         = ENEMY_LAYER_MASK;

        // brute
        constexpr Color   BRUTE_COLOR              = Color{ 255, 100, 255, 255 };
        constexpr Vector3 BRUTE_SIZE               = { 1.5f, 2.5f, 1.5f };
        constexpr int     BRUTE_MIN_HP             = 50;
        constexpr float   BRUTE_SPEED              = 3.5f;
        constexpr int     BRUTE_MELEE_DMG          = 10;
        constexpr float   BRUTE_LOOT_MULTIPLIER    = 1.5f;
        constexpr float   BRUTE_KNOCKBACK_SCALE    = 30.0f;
        constexpr float   BRUTE_KNOCKBACK_DURATION = 0.15f;
        constexpr Layer   BRUTE_LAYER              = ENEMY_LAYER;
        constexpr Layer   BRUTE_LAYER_MASK         = ENEMY_LAYER_MASK;
    }

    // notifications
    namespace notif {
        constexpr float NOTIFICATION_DURATION = 3.0f;

        using Text = std::string_view;
        inline constexpr Text GAIN_EXP             = "+EXP";
        inline constexpr Text GAIN_MONEY           = "+MONEY";
        inline constexpr Text GAIN_LEVELUP         = "+LEVEL";

        inline constexpr Text GAIN_DAMAGE          = "+DAMAGE";
        inline constexpr Text GAIN_ATTACK_SPEED    = "+ATTACK SPEED";
        inline constexpr Text GAIN_MOVE_SPEED      = "+MOVE SPEED";
        inline constexpr Text GAIN_PICKUP_RANGE    = "+PICKUP RANGE";
        inline constexpr Text GAIN_DASH_RANGE      = "+DASH RANGE";
        inline constexpr Text GAIN_HEALTH          = "+HEALTH";
        inline constexpr Text GAIN_MAX_HEALTH      = "+MAX HEALTH";

        inline constexpr Text GAIN_PISTOL          = "+PISTOL";
        inline constexpr Text GAIN_SHOTGUN         = "+SHOTGUN";
        inline constexpr Text GAIN_RIFLE           = "+RIFLE";
        inline constexpr Text GAIN_SNIPER          = "+SNIPER";
        inline constexpr Text GAIN_RAILGUN         = "+RAILGUN";
        inline constexpr Text GAIN_SMG             = "+SMG";
        inline constexpr Text GAIN_GRENADE         = "+GRENADE";
        inline constexpr Text GAIN_ROCKET_LAUNCHER = "+ROCKET LAUNCHER";

        inline constexpr Text GAIN_INVUL           = "+INVULNERABLE";
        inline constexpr Text LOSE_INVUL           = "-INVULNERABLE";
    }
}