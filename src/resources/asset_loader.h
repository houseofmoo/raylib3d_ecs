#pragma once

#include "raylib.h"

namespace rsrc::asset {
    void LoadAssets();
    void UnloadAssets();

    extern Model player_model;

    extern Model grunt_model;
    extern Model brute_model;

    extern Model bullet_model;
    extern Model grenade_model;
    
    extern Model exp_model;
    extern Model money_model;
    extern Model powerup_model;
    extern Model weapon_model;
    extern Model weapon_crate_model;

    extern Music bg_music;
    // extern Sound powerup_fx;
    // extern Sound shoot_fx;
    // extern Sound damage_fx;
    extern Sound sfx_bullet_hit;
    extern Sound sfx_dash;
    extern Sound sfx_explosion;
    extern Sound sfx_melee_hit;
    extern Sound sfx_pistol;
    extern Sound sfx_shotgun;
    extern Sound sfx_sniper;
    extern Sound sfx_pickup;
}