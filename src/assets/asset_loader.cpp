#include "assets/asset_loader.h"
#include "data/entity.h"
#include "data/game/game.h"
#include "raymath.h"
#include "assets.h"
#include "utils/debug.h"

namespace asset {
    Sound LoadSoundAndSetVolume(std::string_view path) {
        Sound s = LoadSound(path.data());
        SetSoundVolume(s, 0.1f);
        return s;
    }

    void LoadSoundAssets() {
        asset::bg_music = LoadMusicStream("assets/music/background.mp3");
        PlayMusicStream(asset::bg_music);
        SetMusicPan(asset::bg_music, 0.5f);
        SetMusicVolume(asset::bg_music, 0.1f);

        asset::sound_fx.Set(
            asset::SoundFxType::Dash, 
            LoadSoundAndSetVolume("assets/soundfx/new/dash.wav")
        );

        asset::sound_fx.Set(
            asset::SoundFxType::MeleeHit, 
            LoadSoundAndSetVolume("assets/soundfx/new/melee_hit.wav")
        );
        asset::sound_fx.Set(
            asset::SoundFxType::BulletHit, 
            LoadSoundAndSetVolume("assets/soundfx/damage.wav")
        );

        asset::sound_fx.Set(
            asset::SoundFxType::Pistol, 
            LoadSoundAndSetVolume("assets/soundfx/new/pistol.wav")
        );
        asset::sound_fx.Set(
            asset::SoundFxType::Shotgun, 
            LoadSoundAndSetVolume("assets/soundfx/new/shotgun.wav")
        );
        asset::sound_fx.Set(
            asset::SoundFxType::Sniper, 
            LoadSoundAndSetVolume("assets/soundfx/new/sniper.wav")
        );
        asset::sound_fx.Set(
            asset::SoundFxType::SMG, 
            LoadSoundAndSetVolume("assets/soundfx/new/pistol.wav")
        );
        asset::sound_fx.Set(
            asset::SoundFxType::GrenadeLauncher, 
            LoadSoundAndSetVolume("assets/soundfx/new/pistol.wav")
        );
        asset::sound_fx.Set(
            asset::SoundFxType::Explosion, 
            LoadSoundAndSetVolume("assets/soundfx/new/explosion.wav")
        );

        asset::sound_fx.Set(
            asset::SoundFxType::Pickup, 
            LoadSoundAndSetVolume("assets/soundfx/new/pickup.wav")
        );
    }

    void UnloadSoundAssets() {
        auto [beg, end] = asset::sound_fx.Iter();
        for (auto it = beg; it != end; ++it) {
            UnloadSound(*it);
        }
        asset::sound_fx.Clear();
    }

    void LoadModelAssets() {
        asset::models.Set(
            asset::ModelType::Player,
            LoadModelFromMesh(
                GenMeshCube(data::cnst::PLAYER_SIZE.x, data::cnst::PLAYER_SIZE.y, data::cnst::PLAYER_SIZE.z)
            )
        );

        asset::models.Set(
            asset::ModelType::Grunt,
            LoadModelFromMesh(
                GenMeshCube(data::cnst::GRUNT_SIZE.x, data::cnst::GRUNT_SIZE.y, data::cnst::GRUNT_SIZE.z)
            )
        );

        asset::models.Set(
            asset::ModelType::Brute,
            LoadModelFromMesh(
                GenMeshCube(data::cnst::BRUTE_SIZE.x, data::cnst::BRUTE_SIZE.y, data::cnst::BRUTE_SIZE.z)
            )
        );

        asset::models.Set(
            asset::ModelType::Bullet,
            LoadModelFromMesh(
                GenMeshCube(data::cnst::BULLET_SIZE.x, data::cnst::BULLET_SIZE.y, data::cnst::BULLET_SIZE.z)
            )
        );

        asset::models.Set(
            asset::ModelType::Grenade,
            LoadModelFromMesh(
                GenMeshCube(data::cnst::GRENADE_SIZE.x, data::cnst::GRENADE_SIZE.y, data::cnst::GRENADE_SIZE.z)
            )
        );

        asset::models.Set(
            asset::ModelType::Exp,
            LoadModelFromMesh(
                GenMeshCube(data::cnst::EXP_SIZE.x, data::cnst::EXP_SIZE.y, data::cnst::EXP_SIZE.z)
            )
        );

        asset::models.Set(
            asset::ModelType::Money,
            LoadModelFromMesh(
                GenMeshCube(data::cnst::MONEY_SIZE.x, data::cnst::MONEY_SIZE.y, data::cnst::MONEY_SIZE.z)
            )
        );

        asset::models.Set(
            asset::ModelType::Powerup,
            LoadModelFromMesh(
                GenMeshCube(data::cnst::POWERUP_SIZE.x, data::cnst::POWERUP_SIZE.y, data::cnst::POWERUP_SIZE.z)
            )
        );

        asset::models.Set(
            asset::ModelType::Weapon,
            LoadModelFromMesh(
                GenMeshCube(data::cnst::WEAPON_SIZE.x, data::cnst::WEAPON_SIZE.y, data::cnst::WEAPON_SIZE.z)
            )
        );

        asset::models.Set(
            asset::ModelType::WeaponCrate,
            LoadModelFromMesh(
                GenMeshCube(data::cnst::WEAPON_CRATE_SIZE.x, data::cnst::WEAPON_CRATE_SIZE.y, data::cnst::WEAPON_CRATE_SIZE.z)
            )
        );
    }
    
    void UnloadModelAssets() {
        auto [beg, end] = asset::models.Iter();
        for (auto it = beg; it != end; ++it) {
            UnloadModel(*it);
        }
        asset::models.Clear();
    }

    void LoadAssets() {
        LoadModelAssets();
        LoadSoundAssets();
    }

    void UnloadAssets() {
        UnloadSoundAssets();
        UnloadModelAssets();
    }
}