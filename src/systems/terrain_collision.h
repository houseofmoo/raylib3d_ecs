// #pragma once

// #include "storage/registry.h"
// #include "components/components.h"
// #include "components/tags.h"
// #include "utils/rl_utils.h"
// #include "utils/profiler.h"

// namespace sys::col {
//     inline void TerrainCollision(Storage::Registry& world) {
//         PROFILE_SCOPE("TerrainCollision()");
//         for (auto terrain : world.View<tag::Terrain,
//                                         cmpt::Collider,
//                                         cmpt::Transform>()) {

//             auto& ttrans = world.GetComponent<cmpt::Transform>(terrain);
//             auto& tcol = world.GetComponent<cmpt::Collider>(terrain);
//             auto tbox = utils::GetBoundingBox(ttrans, tcol);

//             for (auto entity : world.View<cmpt::Collider, cmpt::Transform>()) {

//                 // do these layer interact?
//                 auto& ecol = world.GetComponent<cmpt::Collider>(entity);
//                 if (!data::layer::Interacts(tcol.layer, tcol.mask, ecol.layer, ecol.mask)) {
//                     continue;
//                 }

//                 // if (world.HasComponent<tag::Terrain>(entity)) {
//                 //     continue;
//                 // }

//                 auto& etrans = world.GetComponent<cmpt::Transform>(entity);
//                 auto ebox = utils::GetBoundingBox(etrans, ecol);
                
//                 if (!CheckCollisionBoxes(tbox, ebox)) continue;

//                 // some objects are marked for destruction when they touch terrain
//                 if (world.HasComponent<cmpt::DestroyOnContact>(entity)) {
//                     world.AddComponent<tag::Destroy>(entity);
//                     continue;
//                 }

//                 // update position so this enitiy no longer overlaps with terrain
//                 auto direction = utils::PushbackDirection(ebox, tbox);
//                 etrans.position = Vector3Add(etrans.position, Vector3{ direction.x, 0.0f, direction.z });
//             }
//         }
//     }
// }