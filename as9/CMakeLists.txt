cmake_minimum_required(VERSION 3.18)
project(DuckLib LANGUAGES CXX)
add_subdirectory(raylib-cpp)
file(GLOB sources "src/*.cpp" "src/*.c")
add_executable(as9 ${sources})
target_link_libraries(as9 PUBLIC raylib raylib_cpp raylib::buffered)
set_property(TARGET as9 PROPERTY CXX_STANDARD 20)

configure_file(assets/textures/water.jpg assets/textures/water.jpg COPYONLY)