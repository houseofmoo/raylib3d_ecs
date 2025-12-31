cmake_minimum_required(VERSION 3.15)
project(raylib_3d LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Default build type if not set
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Use build directory for runtime output
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Optional feature
option(PROFILER_ENABLED "Enable function profiling" OFF)

# Project sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.c
)

# Executable target
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Build type: Debug")
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
else()
    message(STATUS "Build type: Release")
    # WIN32 to hide the console window on Windows
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
endif()

# Strict warnings only for non-system code (code in src/)
target_compile_options(${PROJECT_NAME} PRIVATE
    -Wall -Wextra -Wpedantic -Werror -Wunused
)

if (PROFILER_ENABLED AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Profiler: Enabled")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PROFILER_ENABLED)
else()
    message(STATUS "Profiler: Disabled")
endif()

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

# Local headers
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src

    ${CMAKE_CURRENT_SOURCE_DIR}/src/components

    ${CMAKE_CURRENT_SOURCE_DIR}/src/data
    ${CMAKE_CURRENT_SOURCE_DIR}/src/data/game

    ${CMAKE_CURRENT_SOURCE_DIR}/src/debug

    ${CMAKE_CURRENT_SOURCE_DIR}/src/assets
    
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sound

    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/equip
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/equip/weapon
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/system
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/system/camera
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/system/events
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/system/map
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world/enemies
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world/loot
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world/player
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world/projectile
    ${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world/terrain

    ${CMAKE_CURRENT_SOURCE_DIR}/src/storage

    ${CMAKE_CURRENT_SOURCE_DIR}/src/systems
    ${CMAKE_CURRENT_SOURCE_DIR}/src/systems/attack
    ${CMAKE_CURRENT_SOURCE_DIR}/src/systems/camera
    ${CMAKE_CURRENT_SOURCE_DIR}/src/systems/cleanup
    ${CMAKE_CURRENT_SOURCE_DIR}/src/systems/collisions
    ${CMAKE_CURRENT_SOURCE_DIR}/src/systems/events

    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
)

# External system headers
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE
    # raygui + styles
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/raygui/src
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/raygui_styles

    # raylib
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/raylib/src

    # ImGui
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends

    # rlImGui
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/rlImGui
)

# raylib
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# ImGui
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
target_include_directories(IMGUI SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends
)

# rlImGui
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/rlImGui)
target_include_directories(RL_IMGUI SYSTEM PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/rlImGui
)
target_link_libraries(RL_IMGUI PUBLIC IMGUI raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE IMGUI RL_IMGUI)

# Copy assets to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying assets directory"
)

# always copy assets to output directory
# add_custom_target(copy_assets ALL
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#         "${CMAKE_SOURCE_DIR}/assets"
#         "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
#     COMMENT "Copying assets directory"
# )
