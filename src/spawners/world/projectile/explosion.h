#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::proj {
    void Explosion(strg::Registry& world, const Vector3 position, const int damage);
}