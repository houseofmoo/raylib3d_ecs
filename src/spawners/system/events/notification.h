#pragma once

#include <string>
#include "storage/registry.h"
#include "data/entity.h"
#include "components/components.h"

namespace spwn::evt {
    inline void Notification(Storage::Registry& world, const std::string_view msg) {
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
                .countdown = data::notif::NOTIFICATION_DURATION 
            }
        );
    }
}