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

void DrawBoundedModel(raylib::Model& model , raylib::Transform transform, const bool drawBound){
  raylib::Transform backupT = model.transform;
  model.transform = transform;
  model.Draw({});
  if(drawBound){
    model.GetTransformedBoundingBox().Draw();
  }
  model.transform = backupT;
}

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
  raylib::Texture2D model03_tex;

  //Loading files, transforms and misc
  //Plane 1
  model01.Load(mesh_file_path + file_map[PLANE]);
  //Plane 2
  model02.Load(mesh_file_path + file_map[BOAT]);
  //Custom Model
  model03.Load(mesh_file_path + file_map[SKULL]);
  model03_tex.Load(texture_file_path + file_map[SKULL_TEX]);
  model03.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = model03_tex;
  
  //Animated Variables
  float radianX = 0;
  float spinSpeed = 10;
  
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

    //Drawing
    window.BeginDrawing();
      ClearBackground(DARKGRAY);
      DrawFPS(10,10);
      DrawText("Use [SPACE] to toggle bounding boxes", 10, 30, 20, GREEN);
      DrawText("Use [LEFT_ALT] to toggle camera modes", 10, 60, 20, GREEN);
      main_camera.BeginMode();
          DrawGrid(50, 25);
          DrawBoundedModel(model01, model01.transform, drawBounds);
          DrawBoundedModel(model01, raylib::Transform(model01.transform).Translate(raylib::Vector3(-100,100,0)).Scale(1,-1,1).RotateX(radianX), drawBounds);
          DrawBoundedModel(model02, raylib::Transform(model02.transform).Translate(raylib::Vector3(-200, 0,0)), drawBounds);
          DrawBoundedModel(model02, raylib::Transform(model02.transform).Translate(raylib::Vector3(200, 0,0)).RotateY(raylib::Radian(PI/2)).RotateY(raylib::Radian((3*PI)/2)), drawBounds);
          DrawBoundedModel(model02, raylib::Transform(model02.transform).Translate(raylib::Vector3(100, 100,0)).Scale(1,2,1), drawBounds);
          DrawBoundedModel(model03, raylib::Transform(model03.transform).Translate(raylib::Vector3(0, 100,50)).Scale(2,2,2).RotateX(raylib::Radian(-(PI)/3)), drawBounds);
      main_camera.EndMode();
    window.EndDrawing();
  }

  return 0;
}