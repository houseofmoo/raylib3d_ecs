#pragma once
#include "assets/assets.h"
#include "raylib.h"

namespace snd {
    void PlaySoundFxGlobal(asset::SoundFxType type);
    void PlaySoundFxPositional(asset::SoundFxType type, const Vector3 position);
}