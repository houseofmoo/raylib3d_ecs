
#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace sys::enemy {
    void SpawnEnemyInterval(
        Storage::Registry& world, 
        const BoundingBox& boundary, 
        const float delta_time
    );
}