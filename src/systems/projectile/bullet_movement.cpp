// #include "systems/projectile/bullet_movement.h"
// #include "components/components.h"
// #include "components/tags.h"

// namespace sys::proj {
//     void BulletMovement(Storage::Registry& world, const float delta_time) {
//         for (auto bullet : world.View<tag::Bullet, 
//                                      cmpt::Transform, 
//                                      cmpt::Velocity>()) {

//             auto& trans = world.GetComponent<cmpt::Transform>(bullet);
//             auto& vel = world.GetComponent<cmpt::Velocity>(bullet);

//             trans.position.x += vel.x * delta_time;
//             trans.position.y += vel.y * delta_time;
//             trans.position.z += vel.z * delta_time;
//         }
//     }
// }