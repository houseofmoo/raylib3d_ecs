#include "resources/asset_loader.h"
#include "data/entity.h"

namespace rsrc::asset {
    Model player_model;

    Model grunt_model;
    Model brute_model;

    Model bullet_model;

    Model exp_model;
    Model money_model;
    Model powerup_model;
    Model weapon_model;

    Music bg_music;

    Sound powerup_fx;
    Sound shoot_fx;
    Sound damage_fx;;

    void LoadSoundAssets() {
        bg_music = LoadMusicStream("assets/music/background.mp3");
        PlayMusicStream(bg_music);
        SetMusicPan(bg_music, 0.5f);
        SetMusicVolume(bg_music, 0.1f);

        //Sound powerup_fx1 = LoadSound("assets/soundfx/powerup1.wav");
        powerup_fx = LoadSound("assets/soundfx/powerup2.wav");
        //SetSoundVolume(powerup_fx1, 0.1f);
        SetSoundVolume(powerup_fx, 0.1f);

        shoot_fx = LoadSound("assets/soundfx/shoot3.wav");
        SetSoundVolume(shoot_fx, 0.1f);

        damage_fx = LoadSound("assets/soundfx/damage.wav");
        SetSoundVolume(damage_fx, 0.12f);
    }

    void LoadModelAssets() {
        Mesh player_mesh = GenMeshCube(data::cnst::PLAYER_SIZE.x, data::cnst::PLAYER_SIZE.y, data::cnst::PLAYER_SIZE.z);
        player_model = LoadModelFromMesh(player_mesh);
        //player_model = LoadModel("assets/models/robot.glb");
        //player_model = LoadModel("assets/models/robot1.gltf");

        Mesh grunt_mesh = GenMeshCube(data::cnst::GRUNT_SIZE.x, data::cnst::GRUNT_SIZE.y, data::cnst::GRUNT_SIZE.z);
        grunt_model = LoadModelFromMesh(grunt_mesh);

        Mesh brute_mesh = GenMeshCube(data::cnst::BRUTE_SIZE.x, data::cnst::BRUTE_SIZE.y, data::cnst::BRUTE_SIZE.z);
        brute_model = LoadModelFromMesh(brute_mesh);

        Mesh bullet_mesh = GenMeshCube(data::cnst::PROJECTILE_SIZE.x, data::cnst::PROJECTILE_SIZE.y, data::cnst::PROJECTILE_SIZE.z);
        bullet_model = LoadModelFromMesh(bullet_mesh);

        Mesh exp_mesh = GenMeshCube(data::cnst::EXP_SIZE.x, data::cnst::EXP_SIZE.y, data::cnst::EXP_SIZE.z);
        exp_model = LoadModelFromMesh(exp_mesh);

        Mesh money_mesh = GenMeshCube(data::cnst::MONEY_SIZE.x, data::cnst::MONEY_SIZE.y, data::cnst::MONEY_SIZE.z);
        money_model = LoadModelFromMesh(money_mesh);

        Mesh powerup_mesh = GenMeshCube(data::cnst::POWERUP_SIZE.x, data::cnst::POWERUP_SIZE.y, data::cnst::POWERUP_SIZE.z);
        powerup_model = LoadModelFromMesh(powerup_mesh);
        //powerup_model = LoadModel("assets/models/powerup.glb");

        Mesh weapon_mesh = GenMeshCube(data::cnst::WEAPON_SIZE.x, data::cnst::WEAPON_SIZE.y, data::cnst::WEAPON_SIZE.z);
        weapon_model = LoadModelFromMesh(weapon_mesh);
    }
    
    void LoadAssets() {
        LoadModelAssets();
        LoadSoundAssets();
    }

    void UnloadAssets() {
        UnloadMusicStream(bg_music);

        UnloadSound(damage_fx);
        UnloadSound(shoot_fx);
        UnloadSound(powerup_fx);

        UnloadModel(player_model);

        UnloadModel(grunt_model);
        UnloadModel(brute_model);

        UnloadModel(bullet_model);

        UnloadModel(exp_model);
        UnloadModel(money_model);
        UnloadModel(powerup_model);
        UnloadModel(weapon_model);
    }
}