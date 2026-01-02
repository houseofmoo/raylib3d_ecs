// #include "systems/events/notification_event_system.h"
// #include "raylib.h"
// #include "components/components.h"
// #include "raygui_impl.h"

// namespace sys::evt {
//    void DrawNotifications(strg::Registry& world, const float delta_time) noexcept {
//         int y_offset = GetScreenHeight() - 50;  // start from bottom of the screen 
//         int x_pos = 50;                         // left side

//         for (auto noti : world.View<cmpt::Notification, 
//                                     cmpt::Lifetime>()) {
//             auto& lifetime = world.GetComponent<cmpt::Lifetime>(noti);
//             lifetime.countdown -= delta_time;
//             if (lifetime.countdown <= 0.0) {
//                 world.AddComponent<tag::Destroy>(noti);
//                 continue;
//             }
            
//             auto& notification = world.GetComponent<cmpt::Notification>(noti);
//             rgui::Label(
//                 notification.msg.data(), 
//                 Vector2{ static_cast<float>(x_pos), static_cast<float>(y_offset) }
//             );
//             y_offset -= 20;
//         }
//    }
// }