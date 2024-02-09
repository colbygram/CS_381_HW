//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html)
//Problem: Render 3D models to the screen
//Solution: 
//Extra Credit: 

#include <iostream>
#include <unordered_map>
#include "../raylib-cpp/include/raylib-cpp.hpp"

enum MESH_FILES{
  PLANE = 0,
  BOAT,
  TANKER
};

int main(void)
{
  //Constants
  const int WIDTH = 960;
  const int HEIGHT = 540; 
  const std::string texture_file_path = "../assets/textures/";
  const std::string mesh_file_path = "../assets/meshes/";

  std::unordered_map<MESH_FILES, std::string> file_map;
  file_map[PLANE] = "PolyPlane.glb";
  file_map[BOAT] = "SmitHouston_Tug.glb";
  file_map[TANKER] = "OilTanker.glb";

  //Raylib Objects
  raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 1");
  raylib::Camera main_camera(raylib::Vector3(-50,50,50), raylib::Vector3(0,0,0), raylib::Vector3::Up(), 90, CAMERA_PERSPECTIVE);
  //Note: raylib objects feature destructors so don't need to call CloseWindow() and Unload...() whenever objects are used
  raylib::Model model;
  raylib::BoundingBox box;

  model.Load(mesh_file_path + file_map[PLANE]);
  box = GetModelBoundingBox(model);
  while (!window.ShouldClose())
  {
    UpdateCamera(&main_camera, CAMERA_ORBITAL);

    //Begin Rendering
    window.BeginDrawing();
    {
      ClearBackground(GRAY);
      main_camera.BeginMode();
      {
          DrawGrid(100, 5);
          DrawLine3D(raylib::Vector3(0,0,0),raylib::Vector3(10,0,0), RED);//X-axis
          DrawLine3D(raylib::Vector3(0,0,0),raylib::Vector3(0,10,0), YELLOW); //Y-axis
          DrawLine3D(raylib::Vector3(0,0,0),raylib::Vector3(0,0,10), BLUE); // Z-axis
          DrawModel(model, raylib::Vector3(0,3,0), 1, WHITE);
          DrawBoundingBox(box, WHITE);
      }
      main_camera.EndMode();
    }
    window.EndDrawing();
  }

  return 0;
}