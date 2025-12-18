// #include "systems/ai/ai.h"

// #include "raylib.h"
// #include "raymath.h"

// #include "data/player/player.h"
// #include "data/entity/entity_type.h"
// #include "storage/registry.h"
// #include "components/components.h"

// namespace sys::ai {
//     void MoveIntent(Storage::Registry& world) {
//         for (auto entity : world.View<cmpt::MoveIntent, cmpt::Transform>()) {
//             auto& intent = world.GetComponent<cmpt::MoveIntent>(entity);
//             auto& trans = world.GetComponent<cmpt::Transform>(entity);

//             if (world.HasComponent<cmpt::SpawnAnimation>(entity)) {
//                 continue;
//             }
            
//             if (world.HasComponent<cmpt::DeathAnimation>(entity)) {
//                 continue;
//             }

//             // get player position
//             Entity player_id = data::player::g_player.id;
//             auto& ptrans = world.GetComponent<cmpt::Transform>(player_id);

//             switch (intent.type) {
//                 case cmpt::MoveIntentType::Seek: {
//                     intent.direction = Vector3Normalize(
//                         Vector3Subtract(ptrans.position, trans.position)
//                     );
//                     intent.direction.y = 0.0f;
//                     break;
//                 }

//                 case cmpt::MoveIntentType::Random: {
//                     Vector3 dir = Vector3Subtract(ptrans.position, trans.position);
//                     dir = Vector3{
//                         dir.x + GetRandomValue(-100, 100),
//                         dir.y = 0.0f,
//                         dir.z + GetRandomValue(-100, 100),
//                     };
//                     intent.direction = Vector3Normalize(dir);
//                     break;
//                 }

//                 default: {}
//             }
//         }
//     }
// }