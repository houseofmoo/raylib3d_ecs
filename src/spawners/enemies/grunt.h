#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::enemy {
    void Grunt(Storage::Registry& world, const Vector3 position, const int hp);
}