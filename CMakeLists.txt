cmake_minimum_required(VERSION 3.10)
project(orbit-sim)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

# Source files
add_executable(sat_sim
    core/src/sat_sim.cpp
    core/src/satellite.cpp
    core/src/sphere.cpp
    core/src/planet.cpp
    core/src/shader.cpp
    core/src/window.cpp
    core/src/input.cpp
    core/src/utils.cpp
    core/src/camera.cpp
    core/src/gui.cpp
    core/src/skybox.cpp
    core/src/stb.cpp
)

# Define targets
add_library(glad STATIC vendor/src/glad.c)
add_library(imgui STATIC
    extern/imgui/imgui.cpp
    extern/imgui/imgui_draw.cpp
    extern/imgui/imgui_tables.cpp
    extern/imgui/imgui_widgets.cpp
    extern/imgui/backends/imgui_impl_glfw.cpp
    extern/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    extern/imgui
    extern/imgui/backends
)
target_include_directories(glad PUBLIC vendor/include)
target_include_directories(sat_sim PRIVATE core/inc)

target_link_libraries(imgui PRIVATE glfw)
target_link_libraries(sat_sim PRIVATE glad glfw glm::glm imgui OpenGL::GL)

add_custom_command(TARGET sat_sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/shaders  ${CMAKE_BINARY_DIR}/shaders
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/textures ${CMAKE_BINARY_DIR}/textures
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/scripts  ${CMAKE_BINARY_DIR}/scripts
)


# Print Final Configuration
message(STATUS "Configuration Complete!")


#cmake -S . -B build
#cmake --build build
