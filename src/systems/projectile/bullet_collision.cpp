
// #include "systems/projectile/bullet_collision.h"
// #include "raymath.h"
// #include "utils/rl_utils.h"
// #include "components/components.h"
// #include "components/tags.h"
// #include "storage/collision_pairs.h"

// namespace sys::proj {
    
//     CollisionEnterCache bullet_enemy_collision_cache;

//     void BulletEnemyCollision(Storage::Registry& world, const Sound& sound_fx) {
//         for (auto bullet : world.View<tag::Bullet, 
//                                     cmpt::Transform,
//                                     cmpt::Collider,
//                                     cmpt::Damage>()) {

//             auto& btrans = world.GetComponent<cmpt::Transform>(bullet);
//             auto& bcol = world.GetComponent<cmpt::Collider>(bullet);
//             auto& bdam = world.GetComponent<cmpt::Damage>(bullet);
//             auto bbox = utils::GetBoundingBox(btrans, bcol);

//             for (auto enemy : world.View<tag::Enemy, 
//                                         cmpt::Transform, 
//                                         cmpt::Collider,
//                                         cmpt::DamageReceived>()) {

//                 auto& etrans = world.GetComponent<cmpt::Transform>(enemy);
//                 auto& ecol = world.GetComponent<cmpt::Collider>(enemy);
//                 auto ebox = utils::GetBoundingBox(etrans, ecol);

//                 // if not colliding, skip
//                 if (!CheckCollisionBoxes(bbox, ebox)) {
//                     continue;
//                 }

//                 // record this collision
//                 auto col_pair = CollisionPair{ bullet, enemy };
//                 bullet_enemy_collision_cache.current.insert(col_pair);

//                 // if bullet has already collided with this enemy before, skip
//                 if (bullet_enemy_collision_cache.previous.contains(col_pair)) {
//                     continue;
//                 }

//                 auto& dmg = world.GetComponent<cmpt::DamageReceived>(enemy);
//                 dmg.total += bdam.amount;
                
//                 PlaySound(sound_fx);
//                 SetSoundPitch(sound_fx, (float)GetRandomValue(9, 15) * 0.1f);

//                 // may be able to penetrate multiple enemies
//                 bdam.penetration -= 1;
//                 if (bdam.penetration <= 0) {
//                     world.AddComponent<tag::Destroy>(bullet);
//                     break;
//                 }
//             }
//         }

//         // swap prev and current and clear current for next call
//         std::swap(
//             bullet_enemy_collision_cache.previous, 
//             bullet_enemy_collision_cache.current
//         );
//         bullet_enemy_collision_cache.current.clear();
//     }

//     // void BulletTerrainCollision(Storage::Registry& world) {
//     //     for (auto bullet : world.View<tag::Bullet, 
//     //                                 cmpt::Transform,
//     //                                 cmpt::Collider>()) {

//     //         auto& btrans = world.GetComponent<cmpt::Transform>(bullet);
//     //         auto& bcol = world.GetComponent<cmpt::Collider>(bullet);
//     //         auto bbox = utils::GetBoundingBox(btrans, bcol);

//     //         for (auto terrain : world.View<tag::Terrain, 
//     //                                     cmpt::Transform, 
//     //                                     cmpt::Collider>()) {

//     //             auto& ttrans = world.GetComponent<cmpt::Transform>(terrain);
//     //             auto& tcol = world.GetComponent<cmpt::Collider>(terrain);
//     //             auto tbox = utils::GetBoundingBox(ttrans, tcol);

//     //             if (CheckCollisionBoxes(bbox, tbox)) {
//     //                 world.AddComponent<tag::Destroy>(bullet);
//     //                 break;
//     //             }
//     //         }
//     //     }
//     // }
// }