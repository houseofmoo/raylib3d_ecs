#pragma once

#include "storage/registry.h"

namespace sys::evt {
    void DrawNotifications(Storage::Registry& world, const float delta_time) noexcept;
}