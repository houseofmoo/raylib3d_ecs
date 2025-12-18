#pragma once

#include "storage/registry.h"

namespace sys::noti {
    void DrawNotifications(Storage::Registry& world, const float delta_time) noexcept;
}