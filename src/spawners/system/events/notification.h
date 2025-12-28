#pragma once

#include <string>
#include "storage/registry.h"
#include "data/entity.h"
#include "components/components.h"

namespace spwn::evt {
    inline void Notification(strg::Registry& world, const std::string_view msg) {
        Entity noti = world.CreateEntity();

        world.AddComponent<tag::Event>(
            noti, 
            tag::Event{}
        );

        world.AddComponent<cmpt::Notification>(
            noti,
            cmpt::Notification{ 
                .msg = msg
            }
        );

        world.AddComponent<cmpt::Lifetime>(
            noti,
            cmpt::Lifetime{
                .start_time = static_cast<double>(0.0f),
                .countdown = static_cast<double>(data::notif::NOTIFICATION_DURATION)
            }
        );
    }
}