#include "spawners/events/notification.h"
#include "data/notifications/notification.h"
#include "data/entity.h"
#include "components/components.h"

namespace spwn::evt {
    void Notification(Storage::Registry& world, std::string msg) noexcept {
        int index = data::noti::notifications.Add(msg);
        Entity noti = world.CreateEntity();

        world.AddComponent<tag::Event>(noti, tag::Event{});

        world.AddComponent<cmpt::Notification>(
            noti,
            cmpt::Notification{ .notification_index = index }
        );

        world.AddComponent<cmpt::Lifetime>(
            noti,
            cmpt::Lifetime{ .countdown = 3.0f }
        );
    }
}