//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
//Problem: 
//Solution: 
//Extra Credit: 

#include "Resources.hpp"
#include <bitset>

char map[] = "W..............................W"
             "W..............................W"
             "W..............................W"
             "W..............................W"
             "W..............................W"
             "WFFFFFF........................W"
             "W..............................W"
             "W........FFFFFFFF.......FFFFFFFW"
             "W..............................W"
             "W...............FFFFFF.........W"
             "W..............................W"
             "WFFFFFFFFFFFFF.................W"
             "W..............................W"
             "W...................FFFFFFFFFFFW"
             "W..............................W"
             "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

struct Position{
    raylib::Vector2 m_position;
};

struct Tile{
    //Tiles 32x32
    // bits representing whether is a floor or wall or neither.
    //bit 0 = floor
    //bit 1 = wall
    std::bitset<2> m_bit_flags;
    raylib::Color m_color;
};

void MoveSystem(raylib::BufferedInput& inputs){

}

int main(){
    //Consts
    const int WIDTH = 1024;
    const int HEIGHT = 512;
    const std::string texture_path = "assets/textures/";
    
    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 9");
    raylib::Camera2D main_camera(raylib::Vector2::Zero(), raylib::Vector2::Zero(), 0, 1);

    raylib::BufferedInput player_inputs;
    player_inputs["A"] = raylib::Action::key(KEY_A).move();
    player_inputs["D"] = raylib::Action::key(KEY_D).move();
    
    //Game loop
    while (!window.ShouldClose()) {
        player_inputs.PollEvents();
        //Render
        window.BeginDrawing();
            window.ClearBackground(GRAY);
            main_camera.BeginMode();

            main_camera.EndMode();
        window.EndDrawing();
    }
    return 0;
}