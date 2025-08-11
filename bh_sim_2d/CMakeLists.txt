cmake_minimum_required(VERSION 3.28.0)

project(sdl2_t VERSION 0.0.1)

file(GLOB SRC "src/*.cpp")
add_subdirectory("lib/eigen")

add_executable(${CMAKE_PROJECT_NAME} "src/main.cpp")
target_link_libraries(${CMAKE_PROJECT_NAME} SDL2main SDL2 SDL2_gfx)
target_link_libraries(${CMAKE_PROJECT_NAME} Eigen3::Eigen)