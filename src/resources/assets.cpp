#include "resources/assets.h"
#include "data/entity.h"

namespace rsrc::asset {
    Model player_model;
    Model enemy_model;
    Model bullet_model;
    Model loot_model;

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

        Mesh enemy_mesh = GenMeshCube(data::size::ENEMY.x, data::size::ENEMY.y, data::size::ENEMY.z);
        enemy_model = LoadModelFromMesh(enemy_mesh);

        Mesh bullet_mesh = GenMeshCube(data::size::PROJECTILE.x, data::size::PROJECTILE.y, data::size::PROJECTILE.z);
        bullet_model = LoadModelFromMesh(bullet_mesh);

        Mesh loot_mesh = GenMeshCube(data::size::LOOT.x, data::size::LOOT.y, data::size::LOOT.z);
        loot_model = LoadModelFromMesh(loot_mesh);
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
        UnloadModel(enemy_model);
        UnloadModel(bullet_model);
        UnloadModel(loot_model);
    }
}