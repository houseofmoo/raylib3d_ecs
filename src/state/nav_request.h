#pragma once

#include <vector>

namespace state {
    enum class NavRequestKind {
        // request ordered by priority
        None = 0,
        WeaponSelectMenu = 1,
        DeathMenu = 2
    };

    class NavRequest {
        private:
        NavRequestKind request;

        public:
        NavRequest() : request(NavRequestKind::None) {}
        NavRequestKind GetRequest() const noexcept { return request; }
        void MakeRequest(NavRequestKind new_request) noexcept {
            if (static_cast<int>(new_request) > static_cast<int>(request)) {
                request = new_request;
            }
        }
    };
}