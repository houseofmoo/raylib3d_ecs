#include "systems/input/input_system.h"
#include "raymath.h"
#include "components/components.h"
#include "utils/rl_utils.h"
#include "utils/position.h"
#include "utils/debug.h"

namespace sys::input {
    void AIMoveIntent(strg::Registry& world, const float delta_time) {
        PROFILE_SCOPE("AIMoveIntent()");
        // get player position
        Vector3 player_pos = Vector3Zero();
        if (auto ptrans = world.TryGetComponent<cmpt::Transform>(data::g_player.id)) {
            player_pos = ptrans->position;
        }

        for (auto entity : world.View<cmpt::AIMoveIntent, cmpt::Transform>()) {
            if (world.HasComponent<cmpt::SpawnAnimation>(entity)) {
                continue;
            }
            
            if (world.HasComponent<cmpt::DeathAnimation>(entity)) {
                continue;
            }

            auto& intent = world.GetComponent<cmpt::AIMoveIntent>(entity);
            auto& trans = world.GetComponent<cmpt::Transform>(entity);

            switch (intent.mode) {
                case cmpt::AIMoveMode::Melee: {
                    auto& mov = world.GetComponent<cmpt::MeleeMovement>(entity);
                    if (intent.stuck) {
                        // detour in a random direction for 3 seconds
                        mov.detour_countdown = data::cnst::ENEMY_DETOUR_DURATION;
                        mov.detour_direction = utils::DirectionFlattenThenNormalize(
                            trans.position,
                            utils::GetRandomValidPosition()
                        );
                        intent.stuck = false;
                    }

                    if (mov.detour_countdown > 0.0f) {
                        mov.detour_countdown -= delta_time;
                        intent.position = trans.position + (mov.detour_direction * 10.0f);
                        intent.direction = mov.detour_direction;
                    } else {
                        // moves directly at the player
                        intent.position = player_pos;
                        intent.direction = utils::DirectionFlattenThenNormalize(
                            trans.position,
                            player_pos
                        );
                    }
                    break;
                }

                case cmpt::AIMoveMode::Ranged: {
                    // move towards player until theyre some distance from them
                    // move away from player if they're too close
                    break;
                }

                case cmpt::AIMoveMode::Lazy: {
                    // make their way in the general direction of the player
                    auto& lazy = world.GetComponent<cmpt::LazyMovement>(entity);
                    
                    lazy.countdown -= delta_time;
                    if (lazy.countdown > 0.0f && !intent.stuck) continue;

                    intent.stuck = false;
                    lazy.countdown = data::cnst::ENEMY_LAZY_RETARGET_TIME;
                    intent.position = utils::GetRandomValidPositionNearTarget(player_pos, 5);
                    intent.direction = utils::DirectionFlattenThenNormalize(
                        trans.position,
                        intent.position
                    );
                    intent.SetSmoothRotation(trans.rotation);
                    break;
                }

                case cmpt::AIMoveMode::Random: {
                    auto& random = world.GetComponent<cmpt::RandomMovement>(entity);

                    random.countdown -= delta_time;
                    if (random.countdown > 0.0f && !intent.stuck) continue;

                    intent.stuck = false;
                    random.countdown = data::cnst::ENEMY_RANDOM_RETARGET_TIME;
                    intent.position = utils::GetRandomValidPosition();
                    intent.direction = utils::DirectionFlattenThenNormalize(
                        trans.position,
                        intent.position
                    );
                    intent.SetSmoothRotation(trans.rotation);
                    break;
                }

                case cmpt::AIMoveMode::None: {
                    intent.position = { 0.0f, 0.0f, 0.0f };
                    intent.direction = { 0.0f, 0.0f, 0.0f };
                    break;
                }

                default: {}
            }
        }
    }
}