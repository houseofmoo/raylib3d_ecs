#include "spawners/notifications/notification.h"
#include "data/notifications/notification.h"
#include "data/entity.h"
#include "components/components.h"

namespace spwn::noti {
    void Notification(Storage::Registry& world, std::string msg) noexcept {
        int index = data::noti::notifications.Add(msg);
        Entity noti = world.CreateEntity();

        world.AddComponent<cmpt::Notification>(
            noti,
            cmpt::Notification{ .notification_index = index }
        );

        world.AddComponent<cmpt::Transform2D>(
            noti,
            cmpt::Transform2D{ .x= 0.0f, .y = 40.0f }
        );

        world.AddComponent<cmpt::Lifetime>(
            noti,
            cmpt::Lifetime{ .countdown = 3.0f }
        );
    }
}