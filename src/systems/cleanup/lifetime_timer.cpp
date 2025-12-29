#include "systems/cleanup/cleanup.h"
#include "components/components.h"
#include "utils/debug.h"

namespace sys::cu {
    void LifetimeTimer(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("LifetimeTimer()");
        for (auto entity : world.View<cmpt::Lifetime>()) {
            auto& lifetime = world.GetComponent<cmpt::Lifetime>(entity);
            lifetime.countdown -= delta_time;

            if (lifetime.countdown <= 0.0f) {
                world.AddComponent<tag::Destroy>(entity);
            }
        }
    }
}