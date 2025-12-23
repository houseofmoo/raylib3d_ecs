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

    extern Music bg_music;
    extern Sound powerup_fx;
    extern Sound shoot_fx;
    extern Sound damage_fx;;
}