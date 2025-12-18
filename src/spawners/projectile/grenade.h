#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::proj {
    void Grenade(Storage::Registry& world, const Vector3 start_pos,
                 Vector3 end_pos, const int damage);
}