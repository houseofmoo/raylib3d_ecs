#pragma once

#include <array>
#include <string>

namespace data::noti {
    class NotificationList {
        private:
            int m_index;

        public:
            static constexpr int MAX_NOTIFICATIONS = 100;
            std::array<std::string, MAX_NOTIFICATIONS> m_notifications;

        int Add(const std::string_view& message) noexcept {
            int current_index = m_index;
            m_notifications[current_index] = message;
            m_index = (current_index + 1) % MAX_NOTIFICATIONS;
            return current_index;
        }
    };
   
    extern NotificationList notifications;
}