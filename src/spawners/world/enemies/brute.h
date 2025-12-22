#pragma once

#include "raylib.h"
#include "storage/registry.h"
#include "components/components.h"

namespace spwn::enemy {
    void Brute(
        Storage::Registry& world, 
        const Vector3 position, 
        const cmpt::AIMoveMode move_mode, 
        const int hp);
}