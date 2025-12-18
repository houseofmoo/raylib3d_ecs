// #include "systems/enemy/enemy_movement.h"
// #include "raylib.h"
// #include "raymath.h"
// #include "components/components.h"
// #include "components/tags.h"
// #include "utils/rl_utils.h"

// namespace sys::enemy {
//     void EnemyMovement(Storage::Registry& world, const float delta_time) {
//         // get player position
//         Vector3 player_position;
//         for (auto player : world.View<tag::Player,
//                                     cmpt::Transform>()) {
//                 auto& ptrans = world.GetComponent<cmpt::Transform>(player);
//                 player_position = ptrans.position;
//         }

//         // move all enemies towards player
//         for (auto enemy : world.View<tag::Enemy,
//                                     cmpt::Transform,
//                                     cmpt::Velocity,
//                                     cmpt::MoveStats>()) {
            
//             auto& etrans = world.GetComponent<cmpt::Transform>(enemy);
//             auto& evel = world.GetComponent<cmpt::Velocity>(enemy);
//             auto& emov = world.GetComponent<cmpt::MoveStats>(enemy);

//             if (world.HasComponent<cmpt::SpawnAnimation>(enemy)) {
//                 etrans.position.x += evel.x * delta_time;
//                 etrans.position.y += evel.y * delta_time;
//                 etrans.position.z += evel.z * delta_time;
//                 continue;
//             }
            
//             if (world.HasComponent<cmpt::DeathAnimation>(enemy)) {
//                 continue;
//             }
            
//             Vector3 dir = Vector3Subtract(player_position, etrans.position);
//             dir = Vector3Normalize(dir);
//             evel.x = dir.x * emov.speed * emov.speed_multiplier;
//             evel.y = 0.0f;
//             evel.z = dir.z * emov.speed * emov.speed_multiplier;
//             etrans.rotation = utils::GetRotationToLocation(etrans.position, player_position);

//             etrans.position.x += evel.x * delta_time;
//             etrans.position.y += evel.y * delta_time;
//             etrans.position.z += evel.z * delta_time;
//         }
//     }
// }