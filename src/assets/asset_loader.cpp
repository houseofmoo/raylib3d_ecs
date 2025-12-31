#include "assets/asset_loader.h"
#include "data/entity.h"
#include "data/game/game.h"
#include "raymath.h"
#include "assets.h"
#include "utils/debug.h"

namespace asset {
    void LoadSoundFx(asset::SoundFxType type, std::string_view path) {
        asset::sound_fx.Insert(
            type,
            LoadSound(path.data())
        );
        SetSoundVolume(asset::sound_fx[type], 0.1f);
    }

    void LoadSoundAssets() {
        asset::bg_music = LoadMusicStream("assets/music/background.mp3");
        PlayMusicStream(asset::bg_music);
        SetMusicPan(asset::bg_music, 0.5f);
        SetMusicVolume(asset::bg_music, 0.1f);

        LoadSoundFx(asset::SoundFxType::Dash, "assets/soundfx/new/dash.wav");

        LoadSoundFx(asset::SoundFxType::BulletHit, "assets/soundfx/damage.wav");
        LoadSoundFx(asset::SoundFxType::MeleeHit, "assets/soundfx/new/melee_hit.wav");
        
        LoadSoundFx(asset::SoundFxType::Pistol, "assets/soundfx/new/pistol.wav");
        LoadSoundFx(asset::SoundFxType::Shotgun, "assets/soundfx/new/shotgun.wav");
        LoadSoundFx(asset::SoundFxType::Sniper, "assets/soundfx/new/sniper.wav");
        LoadSoundFx(asset::SoundFxType::SMG, "assets/soundfx/new/pistol.wav");
        LoadSoundFx(asset::SoundFxType::GrenadeLauncher, "assets/soundfx/new/pistol.wav");
        LoadSoundFx(asset::SoundFxType::Explosion, "assets/soundfx/new/explosion.wav");

        LoadSoundFx(asset::SoundFxType::PickupExp, "assets/soundfx/new/pickup_small.wav");
        LoadSoundFx(asset::SoundFxType::PickupMoney, "assets/soundfx/new/pickup_small.wav");
        LoadSoundFx(asset::SoundFxType::PickupPowerup, "assets/soundfx/new/pickup_power_up.wav");
        LoadSoundFx(asset::SoundFxType::PickupWeaponCrate, "assets/soundfx/new/pickup_weapon_crate.wav");
    }

    void UnloadSoundAssets() {
        for (auto sound : asset::sound_fx) {
            UnloadSound(sound);
        }
        asset::sound_fx.Clear();
    }

    void ModelFromMesh(const asset::ModelType type, const Vector3 size) {
        asset::models.Insert(
            type,
            LoadModelFromMesh(GenMeshCube(size.x, size.y, size.z))
        );
    }

    void LoadModelAssets() {
        ModelFromMesh(asset::ModelType::Player, data::cnst::PLAYER_SIZE);
        asset::models[asset::ModelType::Player].materials[0].shader = LoadShader(
            "assets/shaders/blue.vs", 
            "assets/shaders/blue.fs"
        );

        ModelFromMesh(asset::ModelType::Grunt, data::cnst::GRUNT_SIZE);
        ModelFromMesh(asset::ModelType::Brute, data::cnst::BRUTE_SIZE);
        ModelFromMesh(asset::ModelType::Bullet, data::cnst::BULLET_SIZE);
        ModelFromMesh(asset::ModelType::Grenade, data::cnst::GRENADE_SIZE);
        ModelFromMesh(asset::ModelType::Exp, data::cnst::EXP_SIZE);
        ModelFromMesh(asset::ModelType::Money, data::cnst::MONEY_SIZE);
        ModelFromMesh(asset::ModelType::Powerup, data::cnst::POWERUP_SIZE);
        ModelFromMesh(asset::ModelType::Weapon, data::cnst::WEAPON_SIZE);
        ModelFromMesh(asset::ModelType::WeaponCrate, data::cnst::WEAPON_CRATE_SIZE);
    }
    
    void UnloadModelAssets() {
        for (auto asset : asset::models) {
            UnloadModel(asset);
        }
        UnloadShader(asset::models[asset::ModelType::Player].materials[0].shader);
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