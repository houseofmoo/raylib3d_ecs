#pragma once

#include "raylib.h"
#include "data/loot.h"
#include "storage/registry.h"

namespace spwn::loot {
    void Loot(
        Storage::Registry& world, 
        const Vector3 position, 
        data::loot::LootKind kind,
        Color color
    );
}