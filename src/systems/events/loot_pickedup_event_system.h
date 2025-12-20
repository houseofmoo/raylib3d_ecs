#pragma once

#include "storage/registry.h"
#include "components/components.h"
#include "utils/debug.h"

namespace sys::evt {
    inline void HandleLootPickedupEvents(Storage::Registry& world) noexcept {
        PROFILE_SCOPE("HandleLootPickedupEvents()");
        for(auto entity : world.View<cmpt::LootEvent>()) {
            PRINT("got loot event");

            world.AddComponent<tag::Destroy>(entity);
        }
    }
}