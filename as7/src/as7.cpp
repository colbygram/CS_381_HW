//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
//Problem: ECS built game
//Solution: 
//Extra Credit: 

#include "Resources.h"
#include "VolumeControl.h"

void PingButton(){
    std::cout << "Ping\n";
}

int main(){
    //Consts
    const int WIDTH = 1920;
    const int HEIGHT = 1080;
    
    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 7");
    raylib::Camera2D main_camera(raylib::Vector2(0,0), raylib::Vector2(0,0), 0 , 1);

    auto gui = InitGuiVolumeControl();

    while (!window.ShouldClose()) {
        //Render
        window.BeginDrawing();
            window.ClearBackground(raylib::Color(15,15,35,255));
            main_camera.BeginMode();
            main_camera.EndMode();
            GuiVolumeControl(&gui);
        window.EndDrawing();
    }
    return 0;
}

#define GUI_VOLUMECONTROL_IMPLEMENTATION
#include "VolumeControl.h"