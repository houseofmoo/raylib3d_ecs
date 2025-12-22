cmake_minimum_required(VERSION 3.15)
project(raylib_3d LANGUAGES C CXX)

# cpp version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# c version
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# use build directory
set(CMAKE_BUILD_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# optional feature
option(PROFILER_ENABLED "Enable function profiling" OFF)

# add all local include files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/components)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/data)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/data/game)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/data/notifications)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/data/player)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/resources)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/equip)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/equip/weapon)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/system)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/system/camera)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/system/events)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/system/map)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world/enemies)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world/loot)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world/player)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world/projectile)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/spawners/world/terrain)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/storage)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/systems)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/systems/attack)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/systems/camera)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/systems/collisions)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/systems/events)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/utils)

# add all local cpp files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/style)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.c
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Build type: Debug")
    add_executable(${PROJECT_NAME} ${SOURCES})
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
    if (PROFILER_ENABLED)
        target_compile_definitions(${PROJECT_NAME} PRIVATE PROFILER_ENABLED)
    endif()
else()
    message(STATUS "Build type: Release")
    add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# add raylib
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/raylib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/raylib/src)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# add raygui
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/raygui/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/raygui/styles)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/raygui/styles/default)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/raygui/styles/dark)

# add OpenGL (for imgui)
#find_package(OpenGL REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)


# add glfw
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw-3.4)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/glfw-3.4/include/GLFW)
#target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# add imgui
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui/backends)
target_link_libraries(${PROJECT_NAME} PRIVATE IMGUI)

# add raylib imgui bridge
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/rlImGui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/rlImGui)
target_link_libraries(${PROJECT_NAME} PRIVATE RL_IMGUI)

# copy assets to output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    COMMENT "Copying assets directory"
)
