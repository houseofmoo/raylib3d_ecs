// #include "systems/projectile/grenade_movement.h"

// #include "raylib.h"
// #include "raymath.h"

// #include "components/components.h"
// #include "components/tags.h"

// namespace sys::proj {
    
//     float EaseInOutQuad(float t) {
//         if (t < 0.5f) {
//             return 2.0f * t * t;
//         }
//         return 1.0f - powf(-2.0f * t + 2.0f, 2.0f) * 0.5f;
//     }

//     void GrenadeMovement(Storage::Registry& world, const float delta_time) {
//         for (auto grenade : world.View<tag::Grenade,
//                                       cmpt::Transform,
//                                       cmpt::Lob>()) {

//             auto& trans = world.GetComponent<cmpt::Transform>(grenade);
//             auto& lob = world.GetComponent<cmpt::Lob>(grenade);

//             lob.lob_time -= delta_time;
//             float u = Clamp(1.0f - lob.lob_time, 0.0f, 1.0f);
//             float u_eased = EaseInOutQuad(lob.lob_time);

//             float max_height = 10.0f;
//             Vector3 horizontal = Vector3Lerp(lob.start_position, lob.end_position, u);
//             horizontal.y += 4.0f * max_height * u * (1.0f - u);
//             trans.position = horizontal;
//         }
//     }
// }