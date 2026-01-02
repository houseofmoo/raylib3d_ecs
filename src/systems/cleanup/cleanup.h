#pragma once

#include "storage/registry.h"
#include "state/nav_request.h"

namespace sys::cu {
    void LifetimeTimer(strg::Registry& world, const float delta_time);
    void OnDestroyEffects(strg::Registry& world);
    void Destroy(strg::Registry& world, state::NavRequest& nav);
}