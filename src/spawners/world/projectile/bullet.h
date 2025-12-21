#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::proj {
    void Bullet(
        Storage::Registry& world, 
        const Vector3 position, 
        const Vector3 direction, 
        const int damage,
        const int penetration
    );
}