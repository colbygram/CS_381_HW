//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
//Problem: ECS built game
//Solution: 
//Extra Credit: 

#include "Resources.h"

int main(){
    //Consts
    const int WIDTH = 1920/2;
    const int HEIGHT = 1080/2;
    
    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 7");
    raylib::Camera3D main_camera(raylib::Vector3(100,50,100),raylib::Vector3(0,0,0), raylib::Vector3(0,1,0), 90, CAMERA_PERSPECTIVE);

    while (!window.ShouldClose()) {
        //Render
        window.BeginDrawing();
            window.ClearBackground(raylib::Color::SkyBlue());
            main_camera.BeginMode();
                DrawGrid(1000, 25);
            main_camera.EndMode();
        window.EndDrawing();
    }
    return 0;
}