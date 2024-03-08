//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
//Problem: Make an ad hoc game
//Solution:  
//Extra Credit: 

#include "Resources.h"
#include "Plane.h"

void inline DrawScore(int posX, int posY, raylib::Vector3 vector);
void inline DrawTitle();
void DrawBoundedModel(raylib::Model& model , raylib::Transform transform);

int main(){
    //Consts
    const int WIDTH = 1920/2;
    const int HEIGHT = 1080/2;
    const std::string mesh_path = "assets/meshes/";
    const std::string texture_path = "assets/textures/";
    const std::string plane_file = "PolyPlane.glb";
    const std::string grass_file = "grass.jpg";
    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 4");
    raylib::Camera3D main_camera(raylib::Vector3(150,150,150), raylib::Vector3(0,0,0), raylib::Vector3(0,1,0), 90, CAMERA_PERSPECTIVE);
    //Grass plane and texture load
    auto plane_mesh = raylib::Mesh::Plane(100'000, 100'000, 200, 200, 100);
    raylib::Model ground_plane = ((raylib::Mesh*)&plane_mesh)->LoadModelFrom();
    raylib::Texture2D grass(texture_path + grass_file);
    grass.SetFilter(TEXTURE_FILTER_BILINEAR);
    grass.SetWrap(TEXTURE_WRAP_REPEAT);
    ground_plane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grass;

    auto cube_mesh = raylib::Mesh::Cube(1, 1, 1);
    raylib::Model cube1 = ((raylib::Mesh*)&cube_mesh)->LoadModelFrom();
    raylib::Model cube2 = ((raylib::Mesh*)&cube_mesh)->LoadModelFrom();
    raylib::Model cube3 = ((raylib::Mesh*)&cube_mesh)->LoadModelFrom();
    raylib::Model cube4 = ((raylib::Mesh*)&cube_mesh)->LoadModelFrom();
    raylib::Model cube5 = ((raylib::Mesh*)&cube_mesh)->LoadModelFrom();
    raylib::Model cube6 = ((raylib::Mesh*)&cube_mesh)->LoadModelFrom();
    raylib::Model cube7 = ((raylib::Mesh*)&cube_mesh)->LoadModelFrom();
    raylib::Model cube8 = ((raylib::Mesh*)&cube_mesh)->LoadModelFrom();
    
    //Vectors for different aspects of the plane
    raylib::Vector3 camera_position(0,10,100);

    //Plane model loaded
    raylib::Model plane_model(mesh_path + plane_file);
    Plane plane01(&plane_model);

    raylib::BoundingBox ground(raylib::Vector3(-1000,-100,-1000),raylib::Vector3(1000,0,1000));
    //floats to store different speeds and maxs
    float camera_speed = 100;
    
    cube1.SetTransform(raylib::Transform(cube1.transform).Translate(raylib::Vector3(100,-450,0)).Scale(25,1000,25));
    cube2.SetTransform(raylib::Transform(cube2.transform).Translate(raylib::Vector3(175,550,0)).Scale(25,1000,25));
    cube3.SetTransform(raylib::Transform(cube3.transform).Translate(raylib::Vector3(250,-460,0)).Scale(25,1000,25));
    cube4.SetTransform(raylib::Transform(cube4.transform).Translate(raylib::Vector3(325,530,0)).Scale(25,1000,25));
    cube5.SetTransform(raylib::Transform(cube5.transform).Translate(raylib::Vector3(400,-480,0)).Scale(25,1000,25));
    cube6.SetTransform(raylib::Transform(cube6.transform).Translate(raylib::Vector3(475,550,0)).Scale(25,1000,25));
    cube7.SetTransform(raylib::Transform(cube7.transform).Translate(raylib::Vector3(550,-480,0)).Scale(25,1000,25));
    cube8.SetTransform(raylib::Transform(cube8.transform).Translate(raylib::Vector3(625,550,0)).Scale(25,1000,25));

    while (!window.ShouldClose())
    { 
        if(IsKeyDown(KEY_SPACE)){
            plane01.started = true;
        }

        if(plane01.CheckCollision(&cube1)){
            plane01.reset = true;
        }
        // else if(plane01.CheckCollision(&cube2)){
        //     plane01.reset = true;
        // }
        // else if(plane01.CheckCollision(&cube3)){
        //     plane01.reset = true;
        // }
        // else if(plane01.CheckCollision(&cube4)){
        //     plane01.reset = true;
        // }
        // else if(plane01.CheckCollision(&cube5)){
        //     plane01.reset = true;
        // }
        // else if(plane01.CheckCollision(&cube6)){
        //     plane01.reset = true;
        // }
        // else if(plane01.CheckCollision(&cube7)){
        //     plane01.reset = true;
        // }
        // else if(plane01.CheckCollision(&cube8)){
        //     plane01.reset = true;
        // }
       
        plane01.Update();

        main_camera.SetPosition(camera_position + plane01.GetPosition());
        main_camera.SetTarget(plane01.GetPosition());

        //Render
        window.BeginDrawing();
            window.ClearBackground(BLUE);
            main_camera.BeginMode();
                cube1.Draw({});
                cube2.Draw({});
                cube3.Draw({});
                cube4.Draw({});
                cube5.Draw({});
                cube6.Draw({});
                cube7.Draw({});
                cube8.Draw({});
                ground_plane.Draw({});
                plane01.Draw();
            main_camera.EndMode();
            if(!plane01.started){
                DrawTitle();
            }else{
                DrawScore(GetRenderWidth()/2,100,plane01.GetPosition());
            }
        window.EndDrawing();
    }
    return 0;
}

void DrawBoundedModel(raylib::Model& model , raylib::Transform transform){
  raylib::Transform backupT = model.transform;
  model.transform = transform;
  model.Draw({});
  model.transform = backupT;
}
void inline DrawScore(int posX, int posY, raylib::Vector3 vector){
    raylib::Text print("", 40 , WHITE, GetFontDefault(), 1.5);
    std::stringstream convert; 
    convert.str(std::string());
    convert<<vector.x;
    print.text = convert.str();
    print.Draw(posX-print.Measure()/2,posY);
}
void inline DrawTitle(){
    DrawText("Flappy Plane", GetRenderWidth()/2 - MeasureText("Flappy Plane",75)/2, 100, 75, YELLOW);
    DrawText("Use [SPACE] to start", GetRenderWidth()/2 - MeasureText("Use [SPACE] to start",20)/2, 200, 20, GREEN);
}
