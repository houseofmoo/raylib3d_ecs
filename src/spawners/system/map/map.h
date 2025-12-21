#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::map {
    void GenerateMap(Storage::Registry& world, const BoundingBox& boundary);
}
