#pragma once

#include "raylib.h"

namespace rsrc::asset {
    void LoadAssets();
    void UnloadAssets();

    extern Model player_model;
    extern Model enemy_model;
    extern Model bullet_model;
    extern Model loot_model;

    extern Music bg_music;
    extern Sound powerup_fx;
    extern Sound shoot_fx;
    extern Sound damage_fx;;
}