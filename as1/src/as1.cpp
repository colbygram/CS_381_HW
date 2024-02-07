//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html)
//Problem: Render 3D models to the screen
//Solution: 
//Extra Credit: 

#include <iostream>
#include "../raylib-cpp/include/raylib-cpp.hpp"

int main(void)
{
  //Constants
  const int WIDTH = 960;
  const int HEIGHT = 540; 
  const std::string texture_file_path = "../assets/textures/";
  const std::string mesh_file_path = "../assets/meshes/";

  //Raylib Objects
  raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 1");
  raylib::Camera main_camera(raylib::Vector3(-3,3,3), raylib::Vector3(0,0,0), raylib::Vector3::Up(), 90, CAMERA_PERSPECTIVE);

  while (!window.ShouldClose())
  {
    int currentHeight = window.GetRenderHeight();
    int currentWidth = window.GetRenderWidth();

    //Begin Rendering
    window.BeginDrawing();
    {
        main_camera.BeginMode();
        {
            DrawLine3D(raylib::Vector3(0,0,0),raylib::Vector3(3,0,0), RED);//X-axis
            DrawLine3D(raylib::Vector3(0,0,0),raylib::Vector3(0,3,0), YELLOW); //Y-axis
            DrawLine3D(raylib::Vector3(0,0,0),raylib::Vector3(0,0,3), BLUE); // Z-axis
        }
        main_camera.EndMode();
    }
    window.EndDrawing();
  }

  CloseWindow();

  return 0;
}