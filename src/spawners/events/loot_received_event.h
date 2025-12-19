#pragma once

#include "storage/registry.h"
#include "data/entity.h"
#include "data/loot.h"
#include "components/components.h"

namespace spwn::evt {
    inline void LootPickedupEvent(Storage::Registry& world, Entity id, data::loot::LootKind kind) noexcept {
        Entity event = world.CreateEntity();
        world.AddComponent<cmpt::LootEvent>(event, cmpt::LootEvent{.id = id, .kind = kind});
    }
}