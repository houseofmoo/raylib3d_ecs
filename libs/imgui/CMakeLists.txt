cmake_minimum_required(VERSION 3.15)
project(IMGUI LANGUAGES C CXX)

# cpp version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# c version
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(IMGUI_SOURCES
    imgui.cpp
    imgui_widgets.cpp
    imgui_draw.cpp
    imgui_demo.cpp
    imgui_tables.cpp
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)

include_directories(backends)
include_directories(./)
include_directories(../GLFW)
add_library(${PROJECT_NAME} STATIC ${IMGUI_SOURCES})