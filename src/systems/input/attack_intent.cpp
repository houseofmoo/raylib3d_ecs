#include "systems/input/input_system.h"
#include "raymath.h"
#include "components/components.h"
#include "utils/debug.h"
#include "utils/rl_utils.h"

namespace sys::input {
    void AttackIntent(strg::Registry& world) {
        PROFILE_SCOPE("AttackIntent()");
        for (auto entity : world.View<cmpt::AttackIntent, cmpt::Transform>()) {
            auto& atk = world.GetComponent<cmpt::AttackIntent>(entity);
            auto& trans = world.GetComponent<cmpt::Transform>(entity);

            if (!atk.active) continue;

            if (auto* input = world.TryGetComponent<cmpt::Input>(entity)) {
                atk.from_position = trans.position;
                atk.to_position = input->mouse_world_position;
                atk.direction = utils::DirectionFlattenThenNormalize(
                    trans.position, 
                    input->mouse_world_position
                );
                continue;
            }
            
            if (auto* input = world.TryGetComponent<cmpt::AIMoveIntent>(entity)) {
                atk.from_position = trans.position;
                atk.to_position = input->position;
                atk.direction = input->direction;
            }
        }
    }
}