#pragma once

#include "storage/registry.h"
#include "state/nav_request.h"

namespace sys::evt {
    void HandleLootPickedupEvents(strg::Registry& world, state::NavRequest& nav) noexcept;
}