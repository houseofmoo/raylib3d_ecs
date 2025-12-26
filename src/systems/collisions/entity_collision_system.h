#pragma once

#include "storage/registry.h"
#include "storage/collision_pairs.h"

namespace sys::col {
    extern strg::CollisionEnterCache collision_cache;
    void EntityCollision(strg::Registry& world);
    void EntityCollision_old(strg::Registry& world);
}