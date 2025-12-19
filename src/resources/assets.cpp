#include "resources/assets.h"
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
        Mesh player_mesh = GenMeshCube(data::size::PLAYER.x, data::size::PLAYER.y, data::size::PLAYER.z);
        player_model = LoadModelFromMesh(player_mesh);
        //player_model = LoadModel("assets/models/robot.glb");
        //player_model = LoadModel("assets/models/robot1.gltf");

        Mesh grunt_mesh = GenMeshCube(data::size::GRUNT.x, data::size::GRUNT.y, data::size::GRUNT.z);
        grunt_model = LoadModelFromMesh(grunt_mesh);

        Mesh brute_mesh = GenMeshCube(data::size::BRUTE.x, data::size::BRUTE.y, data::size::BRUTE.z);
        brute_model = LoadModelFromMesh(brute_mesh);

        Mesh bullet_mesh = GenMeshCube(data::size::PROJECTILE.x, data::size::PROJECTILE.y, data::size::PROJECTILE.z);
        bullet_model = LoadModelFromMesh(bullet_mesh);

        Mesh exp_mesh = GenMeshCube(data::size::EXP.x, data::size::EXP.y, data::size::EXP.z);
        exp_model = LoadModelFromMesh(exp_mesh);

        Mesh money_mesh = GenMeshCube(data::size::MONEY.x, data::size::MONEY.y, data::size::MONEY.z);
        money_model = LoadModelFromMesh(money_mesh);

        Mesh powerup_mesh = GenMeshCube(data::size::POWERUP.x, data::size::POWERUP.y, data::size::POWERUP.z);
        powerup_model = LoadModelFromMesh(powerup_mesh);
        //powerup_model = LoadModel("assets/models/powerup.glb");

        Mesh weapon_mesh = GenMeshCube(data::size::WEAPON.x, data::size::WEAPON.y, data::size::WEAPON.z);
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