#include "systems/cleanup/cleanup.h"
#include "components/components.h"
#include "data/game/game.h"
#include "utils/debug.h"

namespace sys::cu {
    void Destroy(strg::Registry& world) {
        PROFILE_SCOPE("Destroy()");
        for (auto entity : world.View<tag::Destroy>()) {
            if (world.HasComponent<tag::Enemy>(entity)) {
                if (auto* player = world.TryGetComponent<cmpt::Player>(data::g_player.id)) {
                    player->enemies_defeated += 1;
                }
            }

            if (world.HasComponent<cmpt::Player>(entity)) {
                data::g_game.state = data::GameState_E::Dead;
            }

            world.DestroyEntity(entity);
        }
    }
}