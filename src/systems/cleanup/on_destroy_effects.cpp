#include "systems/cleanup/cleanup.h"
#include "components/components.h"
#include "spawners/world/projectile/explosion.h"
#include "assets/assets.h"
#include "sound/sound_player.h"
#include "utils/debug.h"

namespace sys::cu {
    void OnDestroyEffects(strg::Registry& world) {
        PROFILE_SCOPE("OnDestroyEffects()");
        for (auto entity : world.View<tag::Destroy,
                                      cmpt::Transform>()) {

            // check for on destroy effects
            if (world.HasComponent<cmpt::ExplodeOnDestroy>(entity)) {
                auto& exp = world.GetComponent<cmpt::ExplodeOnDestroy>(entity);
                auto& trans = world.GetComponent<cmpt::Transform>(entity);
                spwn::proj::Explosion(
                    world,
                    spwn::proj::ExplosionConfig {
                        .position = trans.position,
                        .start_size = exp.start_size,
                        .end_size = exp.end_size,
                        .damage = exp.damage,
                        .duration = exp.duration,
                        .knockback_scale = exp.knockback_scale,
                        .knockback_duration = exp.knockback_duration,
                        .layer = exp.layer,
                        .mask = exp.mask
                    }
                );
                snd::PlaySoundFxPositional(asset::SoundFxType::Explosion, trans.position);
            }
        }
    }
}