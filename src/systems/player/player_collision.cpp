
// #include "systems/player/player_collision.h"
// #include <string>
// #include "raymath.h"
// #include "components/components.h"
// #include "components/tags.h"
// #include "data/loot/loot_info.h"
// #include "data/player/player.h"
// #include "utils/rl_utils.h"
// #include "spawners/notifications/notification.h"

// namespace sys::player {
//     void PlayerTerrainCollision(Storage::Registry& world) {
//         for (auto player : world.View<tag::Player, 
//                                     cmpt::Transform,
//                                     cmpt::Collider>()) {

//             auto& ptrans = world.GetComponent<cmpt::Transform>(player);
//             auto& pcol = world.GetComponent<cmpt::Collider>(player);
//             auto pbox = utils::GetBoundingBox(ptrans, pcol);

//             for (auto terrain : world.View<tag::Terrain, 
//                                         cmpt::Transform, 
//                                         cmpt::Collider>()) {

//                 auto& ttrans = world.GetComponent<cmpt::Transform>(terrain);
//                 auto& tcol = world.GetComponent<cmpt::Collider>(terrain);
//                 auto tbox = utils::GetBoundingBox(ttrans, tcol);

//                 if (!CheckCollisionBoxes(pbox, tbox)) continue;

//                 auto direction = utils::PushbackDirection(pbox, tbox);
//                 ptrans.position = Vector3Add(ptrans.position, Vector3{ direction.x, 0.0f, direction.z });
//             }
//         }
//     }
// }