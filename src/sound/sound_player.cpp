#include "sound/sound_player.h"
#include "data/game/game.h"
#include "raymath.h"

namespace snd {
    void PlaySoundFxGlobal(asset::SoundFxType type) {
        auto& soundfx = asset::sound_fx[type];
        PlaySound(soundfx);
        SetSoundPitch(soundfx, static_cast<float>(GetRandomValue(9, 15)) * 0.1f);
    }

    void PlaySoundFxPositional(asset::SoundFxType type, const Vector3 position) {
        constexpr float MAX_VOLUME = 0.12f;
        constexpr float THRESHOLD = 10.0f;
        float distance = Vector3Distance(data::g_player.position, position);
        float volume = MAX_VOLUME;
        if (distance > 0.0f) {
            volume = Clamp(MAX_VOLUME * (THRESHOLD / distance), 0.0f, MAX_VOLUME);
        }

        auto& soundfx = asset::sound_fx[type];
        SetSoundVolume(soundfx, volume);
        PlaySound(soundfx);
        SetSoundPitch(soundfx, static_cast<float>(GetRandomValue(9, 15)) * 0.1f);
    }
}