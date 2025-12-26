#pragma once

#include "storage/registry.h"

namespace sys::evt {
    void HandleLootPickedupEvents(strg::Registry& world) noexcept;
}