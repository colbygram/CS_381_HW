cmake_minimum_required(VERSION 3.18)
project(DuckLib LANGUAGES CXX)
add_subdirectory(raylib-cpp)
file(GLOB sources "src/*.cpp" "src/*.c")
add_executable(as7 ${sources})
target_link_libraries(as7 PUBLIC raylib raylib_cpp raygui)
set_property(TARGET as7 PROPERTY CXX_STANDARD 20)

configure_file(assets/meshes/PolyPlane.glb assets/meshes/PolyPlane.glb COPYONLY)
configure_file(assets/meshes/SmitHouston_Tug.glb assets/meshes/SmitHouston_Tug.glb COPYONLY)
configure_file(assets/textures/grass.jpg assets/textures/grass.jpg COPYONLY)