#pragma once

#include <vector>
#include <algorithm>

namespace state {
    enum class NavRequestKind {
        // request ordered by priority
        None = 0,
        WeaponSelectMenu = 1,
        PlayerLevelup = 2,
        DeathMenu = 3
    };

    class NavRequest {
        private:
        std::vector<NavRequestKind> requests;

        public:
        NavRequest() : requests({ 0 }) {}

        std::vector<NavRequestKind>::const_iterator begin() const { return requests.begin(); }
        std::vector<NavRequestKind>::const_iterator end() const { return requests.end(); }

        void MakeRequest(NavRequestKind new_request) noexcept {
            requests.push_back(new_request);
        }

        void PrioritizeRequests() {
            //auto unique = std::unique(requests.begin(), requests.end());
            //requests.erase(unique, requests.end());

            std::sort(requests.begin(), requests.end(), [](NavRequestKind a, NavRequestKind b){
                return static_cast<int>(a) > static_cast<int>(b);
            });
        }
    };
}