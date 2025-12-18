
// #include "systems/enemy/enemy_collision.h"

// #include "raylib.h"
// #include "raymath.h"
// #include "components/components.h"
// #include "components/tags.h"
// #include "utils/rl_utils.h"
// #include "spawners/notifications/notification.h"

// namespace sys::enemy {
//     // void EnemyTerrainCollision(Storage::Registry& world) {
//     //     for (auto enemy : world.View<tag::Enemy, 
//     //                                 cmpt::Transform,
//     //                                 cmpt::Collider>()) {
//     //         auto& etrans = world.GetComponent<cmpt::Transform>(enemy);
//     //         auto& ecol = world.GetComponent<cmpt::Collider>(enemy);
//     //         auto ebox = utils::GetBoundingBox(etrans, ecol);

//     //         for (auto terrain : world.View<tag::Terrain, 
//     //                                     cmpt::Transform, 
//     //                                     cmpt::Collider>()) {

//     //             auto& ttrans = world.GetComponent<cmpt::Transform>(terrain);
//     //             auto& tcol = world.GetComponent<cmpt::Collider>(terrain);
//     //             auto tbox = utils::GetBoundingBox(ttrans, tcol);

//     //             if (!CheckCollisionBoxes(ebox, tbox)) continue;

//     //             auto direction = utils::PushbackDirection(ebox, tbox);
//     //             etrans.position = Vector3Add(etrans.position, Vector3{ direction.x, 0.0f, direction.z });
//     //         }
//     //     }
//     // }

//     void EnemyPlayerCollision(Storage::Registry& world) {
//         for (auto player : world.View<tag::Player, 
//                                     cmpt::Transform,
//                                     cmpt::Collider,
//                                     cmpt::DamageReceiver>()) {

//             auto& ptrans = world.GetComponent<cmpt::Transform>(player);
//             auto& pcol = world.GetComponent<cmpt::Collider>(player);
//             auto pbox = utils::GetBoundingBox(ptrans, pcol);

//             for (auto enemy : world.View<tag::Enemy, 
//                                         cmpt::Transform, 
//                                         cmpt::Collider,
//                                         cmpt::DamageDealer>()) {

//                 auto& etrans = world.GetComponent<cmpt::Transform>(enemy);
//                 auto& ecol = world.GetComponent<cmpt::Collider>(enemy);
//                 auto ebox = utils::GetBoundingBox(etrans, ecol);

//                 if (!CheckCollisionBoxes(pbox, ebox)) continue;

//                 // push player out of enemy
//                 auto direction = utils::PushbackDirection(pbox, ebox);
//                 ptrans.position = Vector3Add(ptrans.position, Vector3{ direction.x, 0.0f, direction.z });

//                 // check if player can be hurt
//                 if (world.HasComponent<cmpt::Invulnerable>(player)) continue;

//                 auto& edmg = world.GetComponent<cmpt::DamageDealer>(enemy);
//                 auto& pdmg_acc = world.GetComponent<cmpt::DamageReceiver>(player);
//                 pdmg_acc.total += edmg.amount;

//                 world.AddComponent<cmpt::Invulnerable>(player, cmpt::Invulnerable{ 1.0f });
//                 spwn::noti::Notification(
//                     world, 
//                     std::string("- " + std::to_string(edmg.amount) + " HP")
//                 );
//                 spwn::noti::Notification(
//                     world, 
//                     std::string("+INVUL")
//                 );
//             }
//         }
//     }
// }