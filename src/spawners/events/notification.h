#pragma once

#include <string>
#include "storage/registry.h"

namespace spwn::evt {
    void Notification(Storage::Registry& world, std::string msg) noexcept;
}