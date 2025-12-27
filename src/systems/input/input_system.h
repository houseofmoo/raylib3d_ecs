#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace sys::input {
    void PlayerInput(strg::Registry& world, Camera3D& camera);
    void AIMoveIntent(strg::Registry& world, const float delta_time);
    void AttackIntent(strg::Registry& world);
}