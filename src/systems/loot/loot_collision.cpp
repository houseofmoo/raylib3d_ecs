
// #include "systems/loot/loot_collision.h"

// #include "raymath.h"
// #include "components/components.h"
// #include "components/tags.h"
// #include "data/loot.h"
// #include "data/player/player.h"
// #include "spawners/notifications/notification.h"
// #include "utils/rl_utils.h"

// namespace sys::loot {
//     void LootPlayerCollision(Storage::Registry& world, const Sound& sound_fx) {
//         for (auto player : world.View<tag::Player, 
//                                     cmpt::Transform,
//                                     cmpt::Collider>()) {

//             auto& ptrans = world.GetComponent<cmpt::Transform>(player);
//             auto& pcol = world.GetComponent<cmpt::Collider>(player);
//             auto pbox = utils::GetBoundingBox(
//                 ptrans, 
//                 cmpt::Collider{ 
//                     .offset = pcol.offset,
//                     .size = Vector3Scale(pcol.size, data::player::g_player.pickup_range_multiplier) 
//                 }
//             );


//             for (auto powerup : world.View<cmpt::Loot, 
//                                         cmpt::Transform, 
//                                         cmpt::Collider>()) {

//                 auto& putrans = world.GetComponent<cmpt::Transform>(powerup);
//                 auto& pucol = world.GetComponent<cmpt::Collider>(powerup);
//                 auto pubox = utils::GetBoundingBox(putrans, pucol);
                
//                 if (!CheckCollisionBoxes(pbox, pubox)) continue;
                
//                 auto& pukind = world.GetComponent<cmpt::Loot>(powerup);
//                 world.AddComponent<tag::Destroy>(powerup);
                
//                 PlaySound(sound_fx);
//                 switch (pukind.kind) {
//                     case data::loot::LootKind::Exp: {
//                         data::player::g_player.exp += 1;
//                         spwn::noti::Notification(world, std::string("+1 EXP"));
//                         break;
//                     }

//                     case data::loot::LootKind::Damage: {
//                         data::player::g_player.damage_multiplier += 0.1f;
//                         spwn::noti::Notification(world, std::string("+DMG"));
//                         break;
//                     }

//                     case data::loot::LootKind::PelletCount: {
//                         // TODO: this needs to not be a search
//                         for (auto weapons : world.View<tag::Weapon, cmpt::WeaponStats>()) {
//                             auto& wstats = world.GetComponent<cmpt::WeaponStats>(weapons);
//                             if (wstats.parent != player) continue;
                            
//                             if (wstats.pellet_count < 5) {
//                                 wstats.pellet_count += 1;
//                                 spwn::noti::Notification(world, std::string("+PELLETS"));
//                             } else {
//                                 // when pellet count is maxed out
//                                 data::player::g_player.exp += 1;
//                                 spwn::noti::Notification(world, std::string("PELLETS MAX, +EXP"));
//                             }
//                             break;
//                         }
//                         break;
//                     }

//                     case data::loot::LootKind::AttackSpeed: {
//                         data::player::g_player.attack_speed_multiplier += 0.1f;
//                         spwn::noti::Notification(world, std::string("+ATK SPD"));
//                         break;
//                     }

//                     case data::loot::LootKind::Penetration: {
//                         // TODO: this needs to not be a search
//                         for (auto weapons : world.View<tag::Weapon, cmpt::WeaponStats>()) {
//                             auto& wstats = world.GetComponent<cmpt::WeaponStats>(weapons);
//                             if (wstats.parent != player) continue;
                            
//                             if (wstats.penetration < 3) {
//                                 wstats.penetration += 1;
//                                 spwn::noti::Notification(world, std::string("+PENETRATION"));
//                             } else {
//                                 // when penetration is maxed out
//                                 data::player::g_player.exp += 1;
//                                 spwn::noti::Notification(world, std::string("PENETRATION MAX, +EXP"));
//                             }
//                             break;
//                         }
//                     }
//                     default: { break; }
//                 }
//             }
//         }
//     }
// }