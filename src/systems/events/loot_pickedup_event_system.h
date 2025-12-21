#pragma once

#include "storage/registry.h"

namespace sys::evt {
    void HandleLootPickedupEvents(Storage::Registry& world) noexcept;
}