#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace sys::atk {
    void WeaponAttacks(strg::Registry& world, const float delta_time, Sound& sound_fx);
}