// #include "data/loot.h"
// #include "data/player/player.h"
// #include "components/components.h"
// #include "components/tags.h"
// #include "spawners/notifications/notification.h"

// namespace sys::cleanup {
//     // void RemoveStatsusEffects(Storage::Registry& world) {
//     //     for (auto entity : world.View<cmpt::Invulnerable>()) {
//     //         auto& invuln = world.GetComponent<cmpt::Invulnerable>(entity);
//     //         invuln.countdown -= GetFrameTime();

//     //         if (invuln.countdown <= 0.0f) {
//     //             world.RemoveComponent<cmpt::Invulnerable>(entity);
//     //             spwn::noti::Notification(
//     //                 world, 
//     //                 std::string("-INVUL")
//     //             );
//     //         }
//     //     }
//     // }

    

//     inline void Cleanup(Storage::Registry& world, const float delta_time) {
//         for (auto entity : world.View<cmpt::Lifetime>()) {
//             auto& lifetime = world.GetComponent<cmpt::Lifetime>(entity);
//             lifetime.countdown -= delta_time;

//             if (lifetime.countdown <= 0.0f) {
//                 world.DestroyEntity(entity);
//             }
//         }
        
//         for (auto entity : world.View<tag::Destroy>()) {
//             // if (world.HasComponent<cmpt::DeathAnimation>(entity)) {
//             //     auto& death_anim = world.GetComponent<cmpt::DeathAnimation>(entity);
//             //     death_anim.elapsed += delta_time;
//             //     if (death_anim.elapsed < death_anim.duration) {
//             //         continue;
//             //     }
//             //     // world.RemoveComponent<cmpt::DeathAnimation>(entity);
//             //     // world.AddComponent<tag::Destroy>(entity);
//             // }
//             if (world.HasComponent<tag::Enemy>(entity)) {
//                 data::player::g_player.enemies_defeated += 1;
//             }

//             world.DestroyEntity(entity);
//         }
//     }
// }