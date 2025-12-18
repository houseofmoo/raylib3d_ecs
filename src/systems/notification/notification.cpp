#include "systems/notification/notification.h"
#include "raylib.h"
#include "components/components.h"
#include "components/tags.h"
#include "data/notifications/notification.h"

namespace sys::noti {
   void DrawNotifications(Storage::Registry& world, const float delta_time) noexcept {
        int y_offset = GetScreenHeight() - 50;  // start from bottom of the screen 
        int x_pos = 50;                         // left side
        for (auto noti : world.View<cmpt::Notification, 
                                    cmpt::Transform2D,
                                    cmpt::Lifetime>()) {

            auto& lifetime = world.GetComponent<cmpt::Lifetime>(noti);\
            lifetime.countdown -= delta_time;
            if (lifetime.countdown <= 0.0) {
                world.AddComponent<tag::Destroy>(noti);
                continue;
            }

            auto& notification = world.GetComponent<cmpt::Notification>(noti);
            auto& transform = world.GetComponent<cmpt::Transform2D>(noti);
            auto msg = data::noti::notifications.m_notifications[notification.notification_index];

            // TODO: move msg up over time
            DrawText(msg.c_str(), x_pos, y_offset, 20, WHITE);
            y_offset -= 20;
        }
   }
}