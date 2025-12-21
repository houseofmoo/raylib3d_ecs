#pragma once

#include "raylib.h"
#include "storage/registry.h"

namespace spwn::terrain {
    void Terrain(
        Storage::Registry& world, 
        const Vector3 position, 
        const Vector3 size, 
        const bool visible = true
    );

    void Terrain2(
        Storage::Registry& world, 
        const Vector3 position, 
        const Vector3 size, 
        const bool visible = true
    );
}