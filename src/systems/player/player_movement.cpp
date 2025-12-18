// #include "systems/player/player_movement.h"
// #include "components/components.h"
// #include "components/tags.h"
// #include "data/player/player.h"
// #include "utils/rl_utils.h"

// namespace sys::player {
//     void PlayerMovement(Storage::Registry& world, const float delta_time) {
//         for (auto player : world.View<tag::Player,
//                                     cmpt::Transform,
//                                     cmpt::Velocity, 
//                                     cmpt::MoveStats,
//                                     cmpt::Input>()) {

//             auto& trans = world.GetComponent<cmpt::Transform>(player);
//             auto& vel = world.GetComponent<cmpt::Velocity>(player);
//             auto& mov = world.GetComponent<cmpt::MoveStats>(player);
//             auto& input = world.GetComponent<cmpt::Input>(player);
            
//             float dash_multiplier = 1.0f;
//             if (world.HasComponent<cmpt::Dash>(player)) {
//                 auto& dash = world.GetComponent<cmpt::Dash>(player);
//                 dash_multiplier = mov.dash_multiplier;
//                 dash.countdown -= delta_time;

//                 if (dash.countdown <= 0.0f) {
//                     world.RemoveComponent<cmpt::Dash>(player);
//                 }
//             }

//             vel.x = input.direction.x * mov.speed * mov.speed_multiplier * data::player::g_player.move_speed_multiplier * dash_multiplier;
//             vel.y = 0.0f;
//             vel.z = input.direction.z * mov.speed * mov.speed_multiplier * data::player::g_player.move_speed_multiplier * dash_multiplier;
//             trans.rotation = utils::GetRotationToLocation(trans.position, input.mouse_world_position);

//             trans.position.x += vel.x * delta_time;
//             trans.position.y += vel.y * delta_time;
//             trans.position.z += vel.z * delta_time;
//         }
//     }
// }