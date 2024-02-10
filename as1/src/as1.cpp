//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html)
//Problem: Render 3D models to the screen
//Solution: Load models using file paths, load textures if applicable, apply transformations, draw models, draw bounding boxes.
//Extra Credit: Skull mesh added, spinning plane even though kind of bad, toggleable bounding boxes if that counts, toggleable camera modes if that counts

#include <iostream>
#include <unordered_map>
#include "../raylib-cpp/include/raylib-cpp.hpp"

enum MESH_FILES{
  PLANE = 0,
  BOAT,
  TANKER, 
  SKULL,
  SKULL_TEX
};

int main(void)
{
  //Constants
  const int WIDTH = 960;
  const int HEIGHT = 540; 
  const std::string texture_file_path = "../assets/textures/";
  const std::string mesh_file_path = "../assets/meshes/";

  //File map
  std::unordered_map<MESH_FILES, std::string> file_map;
  file_map[PLANE] = "PolyPlane.glb";
  file_map[BOAT] = "SmitHouston_Tug.glb";
  file_map[TANKER] = "OilTanker.glb";
  file_map[SKULL] = "12140_Skull_v3_L2.obj";
  file_map[SKULL_TEX] = "Skull.jpg";

  //Raylib Objects
  raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 1");
  raylib::Camera main_camera(raylib::Vector3(0,100,250), raylib::Vector3(0,75,-10), raylib::Vector3::Up(), 90, CAMERA_PERSPECTIVE);
  //Note: raylib objects feature destructors so don't need to call CloseWindow() and Unload...() whenever objects are used
  raylib::Model model01, model02, model03, model04, model05, model06;
  raylib::Texture2D model06_tex;

  //Loading files, transforms and misc
  //Plane 1
  model01.Load(mesh_file_path + file_map[PLANE]);
  //Plane 2
  model02.Load(mesh_file_path + file_map[PLANE]);
  model02.SetTransform(raylib::Transform(model02.transform).Scale(1,-1,1));
  model02.SetTransform(raylib::Transform(model02.transform).Translate(raylib::Vector3(-100,100,0)));
  //Boat 1
  model03.Load(mesh_file_path + file_map[BOAT]);
  model03.SetTransform(raylib::Transform(model03.transform).Translate(raylib::Vector3(-200, 0,0)));
  //Boat 2
  model04.Load(mesh_file_path + file_map[BOAT]);
  model04.SetTransform(raylib::Transform(model04.transform).Translate(raylib::Vector3(200, 0,0)));
  model04.SetTransform(raylib::Transform(model04.transform).RotateY(raylib::Radian(PI/2)));
  //Boat 3
  model05.Load(mesh_file_path + file_map[BOAT]);
  model05.SetTransform(raylib::Transform(model05.transform).Translate(raylib::Vector3(100, 100,0)));
  model05.SetTransform(raylib::Transform(model05.transform).Scale(1,2,1));
  model05.SetTransform(raylib::Transform(model05.transform).RotateY(raylib::Radian((3*PI)/2)));
  //Custom Model
  model06.Load(mesh_file_path + file_map[SKULL]);
  model06_tex.Load(texture_file_path + file_map[SKULL_TEX]);
  model06.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = model06_tex;
  model06.SetTransform(raylib::Transform(model06.transform).Translate(raylib::Vector3(0, 100,50)));
  model06.SetTransform(raylib::Transform(model06.transform).Scale(2,2,2));
  model06.SetTransform(raylib::Transform(model06.transform).RotateX(raylib::Radian(-(PI)/3)));

  //Animated Variables
  float radianX = 0;
  float spinSpeed = 0.75;
  //Misc Variables
  bool drawBounds = false;
  const int MAX_MODES = 2;
  CameraMode camera_modes[MAX_MODES] = {CAMERA_ORBITAL, CAMERA_FIRST_PERSON};
  int index = 0;
  //Game Loop
  while (!window.ShouldClose())
  {
    //Toggle camera mode
    if(IsKeyPressed(KEY_LEFT_ALT)){
      index = (index+1) % MAX_MODES;
      std::cout << index << std::endl;
    }
    UpdateCamera(&main_camera, camera_modes[index]);

    //Toggle Bounding Boxes
    if(IsKeyPressed(KEY_SPACE)){
      drawBounds = !drawBounds;
    }

    //Animated Objects
    //Plane 2 spin on X-axis
    if(radianX != (2*PI)){
      radianX += (PI/6) * GetFrameTime() * spinSpeed;
    }else{
      radianX = 0;
    }
    model02.SetTransform(raylib::Transform(model02.transform).RotateX(raylib::Radian(radianX)));

    //Drawing
    window.BeginDrawing();
      ClearBackground(DARKGRAY);
      DrawFPS(10,10);
      DrawText("Use [SPACE] to toggle bounding boxes", 10, 30, 20, GREEN);
      DrawText("Use [LEFT_ALT] to toggle camera modes", 10, 60, 20, GREEN);
      main_camera.BeginMode();
          DrawGrid(50, 25);
          model01.Draw(raylib::Vector3(0,0,0), 1, WHITE);
          model02.Draw(raylib::Vector3(0, 0, 0), 1, WHITE);
          model03.Draw(raylib::Vector3(0,0,0), 1, WHITE);
          model04.Draw(raylib::Vector3(0,0,0), 1, WHITE);
          model05.Draw(raylib::Vector3(0,0,0), 1, WHITE);
          model06.Draw(raylib::Vector3(0,0,0), 1, WHITE);
          if(drawBounds){
            DrawBoundingBox(model01.GetTransformedBoundingBox(), WHITE);
            DrawBoundingBox(model02.GetTransformedBoundingBox(), WHITE);
            DrawBoundingBox(model03.GetTransformedBoundingBox(), WHITE);
            DrawBoundingBox(model04.GetTransformedBoundingBox(), WHITE);
            DrawBoundingBox(model05.GetTransformedBoundingBox(), WHITE);
            DrawBoundingBox(model06.GetTransformedBoundingBox(), WHITE);
          }
      main_camera.EndMode();
    window.EndDrawing();
  }

  return 0;
}