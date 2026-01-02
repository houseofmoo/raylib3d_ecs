#include "systems/cleanup/cleanup.h"
#include "components/components.h"
#include "data/global_data.h"
#include "utils/debug.h"

namespace sys::cu {
    void Destroy(strg::Registry& world, state::NavRequest& nav) {
        PROFILE_SCOPE("Destroy()");
        for (auto entity : world.View<tag::Destroy>()) {
            if (world.HasComponent<tag::Enemy>(entity)) {
                if (auto* player = world.TryGetComponent<cmpt::Player>(gd::player.id)) {
                    player->enemies_defeated += 1;
                }
            }

            if (world.HasComponent<cmpt::Player>(entity)) {
                nav.MakeRequest(state::NavRequestKind::DeathMenu);
                continue; // do not actually destroy the player
            }

            world.DestroyEntity(entity);
        }
    }
}