#pragma once

#include "raylib.h"
#include "assets/asset_list.h"

namespace asset {
    enum class ModelType {
        None,
        Player,
        Grunt,
        Brute,

        Bullet,
        Grenade,

        Exp,
        Money,
        Powerup,
        Weapon,
        WeaponCrate
    };

    enum class SoundFxType {
        None,
        Dash,

        BulletHit,
        MeleeHit,

        Pistol,
        Shotgun,
        Sniper,
        Rifle,
        SMG,
        GrenadeLauncher,
        Explosion,

        Pickup
    };

    extern Music bg_music;
    extern AssetList<Sound, SoundFxType> sound_fx;
    extern AssetList<Model, ModelType> models;
}