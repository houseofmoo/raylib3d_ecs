// #include "systems/player/player_input.h"
// #include "raymath.h"
// #include "components/components.h"
// #include "components/tags.h"

// namespace sys::player {
//     bool RayPlaneIntersection(const Ray& ray, float planeY, Vector3& outPoint) {
//         if (fabs(ray.direction.y) < 0.0001f)
//             return false; // Parallel

//         float t = (planeY - ray.position.y) / ray.direction.y;
//         if (t < 0.0f)
//             return false; // Behind camera

//         outPoint = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
//         return true;
//     }

//     void PlayerInput(Storage::Registry& world, Camera3D& camera) {
//         for (auto entity : world.View<tag::Player, 
//                                     cmpt::Input>()) {

//             auto& input = world.GetComponent<cmpt::Input>(entity);

//             input.direction = { 0.0f, 0.0f };
//             if (IsKeyDown(KEY_W)) input.direction.z -= 1.0f;
//             if (IsKeyDown(KEY_S)) input.direction.z += 1.0f;
//             if (IsKeyDown(KEY_A)) input.direction.x -= 1.0f;
//             if (IsKeyDown(KEY_D)) input.direction.x += 1.0f;
//             input.direction = Vector3Normalize(input.direction);

//             // get mouse position on ground plane
//             Vector3 mouse_world_position;
//             Ray ray = GetMouseRay(GetMousePosition(), camera);
//             if (RayPlaneIntersection(ray, 0.0f, mouse_world_position)) {
//                 input.mouse_world_position = mouse_world_position;
//                 // std::cout << "mos: [" << input.mos_world_position.x << ", ";
//                 // std::cout << input.mouse_world_position.y << ", ";
//                 // std::cout << input.mouse_world_position.z << "] " << std::endl;
//             }

//             // dash
//             if (IsKeyPressed(KEY_SPACE) && !world.HasComponent<cmpt::Dash>(entity)) {
//                 world.AddComponent<cmpt::Dash>(entity, cmpt::Dash{ 0.1f });
//             }
//         }
//     }
// }