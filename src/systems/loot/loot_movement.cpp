// #include "systems/loot/loot_movement.h"
// #include "raylib.h"
// #include "raymath.h"
// #include "components/components.h"
// #include "components/tags.h"

// namespace sys::loot {
//     void LootMovement(Storage::Registry& world, const float delta_time) {
//         for (auto powerup : world.View<cmpt::Loot,
//                                         cmpt::Transform>()) {

//             auto& trans = world.GetComponent<cmpt::Transform>(powerup);

//             Quaternion dq = QuaternionFromAxisAngle({0.0f, 1.0f, 0.0f}, 2.0f * delta_time);
//             trans.rotation = QuaternionMultiply(dq, trans.rotation);
//             trans.rotation = QuaternionNormalize(trans.rotation);
//         }
//     }
// }