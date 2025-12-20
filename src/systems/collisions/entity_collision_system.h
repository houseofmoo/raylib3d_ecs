#pragma once

#include "storage/registry.h"
#include "storage/collision_pairs.h"

namespace sys::col {
    extern strg::CollisionEnterCache collision_cache;
    void EntityCollision(Storage::Registry& world);
    void EntityCollision_old(Storage::Registry& world);
}