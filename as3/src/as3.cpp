//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
//Problem: Render multiple plane models, skybox, and ground. Add controls to the planes to simulate plane behavior and be able to switch between the planes
//Solution: Use designated inputs, vector math and physics to allow the plane to move in a semi realistic behavior, 
//Extra Credit: 

#include "Resources.h"
#include "Plane.h"

void inline DrawVector3Text(std::string title ,int posX, int posY, raylib::Vector3 vector);
void inline DrawControls();

int main(){
    //Consts
    const int WIDTH = 1920/2;
    const int HEIGHT = 1080/2;
    const std::string mesh_path = "assets/meshes/";
    const std::string texture_path = "assets/textures/";
    const std::string plane_file = "PolyPlane.glb";
    const std::string grass_file = "grass.jpg";
    const std::string skybox_file = "skybox.png";
    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 3");
    raylib::Camera3D main_camera(raylib::Vector3(150,150,150), raylib::Vector3(0,0,0), raylib::Vector3(0,1,0), 90, CAMERA_PERSPECTIVE);
    //Skybox load
    cs381::SkyBox skybox(texture_path + skybox_file);
    //Grass plane and texture load
    auto plane_mesh = raylib::Mesh::Plane(10'000, 10'000, 50, 50, 25);
    raylib::Model ground_plane = ((raylib::Mesh*)&plane_mesh)->LoadModelFrom();
    raylib::Texture2D grass(texture_path + grass_file);
    grass.SetFilter(TEXTURE_FILTER_BILINEAR);
    grass.SetWrap(TEXTURE_WRAP_REPEAT);
    ground_plane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grass;
    
    //Vectors for different aspects of the plane
    raylib::Vector3 camera_position(100,100,100);

    //Plane model loaded
    raylib::Model plane_model(mesh_path + plane_file);
    Plane plane01(&plane_model), plane02(&plane_model), plane03(&plane_model);
    plane01.name = "Plane 01";
    plane02.name = "Plane 02";
    plane03.name = "Plane 03";
    plane01.SetNextPlane(&plane02);
    plane02.SetNextPlane(&plane03);
    plane03.SetNextPlane(&plane01);
    Plane* current_plane = &plane01;
    current_plane->SetSelection(true);

    raylib::BoundingBox ground(raylib::Vector3(-1000,-100,-1000),raylib::Vector3(1000,0,1000));
    //floats to store different speeds and maxs
    float camera_speed = 100;

    while (!window.ShouldClose())
    { 
        if(IsKeyPressed(KEY_TAB) && current_plane->GetNextPlane() != nullptr){
            current_plane->SetSelection(false);
            current_plane = current_plane->GetNextPlane();
            current_plane->SetSelection(true);
        }
        plane01.Update();
        plane02.Update();
        plane03.Update();
        //Camera input handling using arrow keys
        if(IsKeyDown(KEY_RIGHT)){
            camera_position.x += camera_speed * GetFrameTime();
        }
        if(IsKeyDown(KEY_LEFT)){
            camera_position.x -= camera_speed * GetFrameTime();
        }
        if(IsKeyDown(KEY_UP)){
            camera_position.y += camera_speed * GetFrameTime();
        }
        if(IsKeyDown(KEY_DOWN)){
            camera_position.y -= camera_speed * GetFrameTime();
        }
        main_camera.SetPosition(camera_position + current_plane->GetPosition());
        main_camera.SetTarget(current_plane->GetPosition());

        //Render
        window.BeginDrawing();
            window.ClearBackground(GRAY);
            main_camera.BeginMode();
                skybox.Draw();
                ground_plane.Draw({});
                plane01.Draw();
                plane02.Draw();
                plane03.Draw();
            main_camera.EndMode();
            DrawControls();
            DrawVector3Text("Position: ",10,10,current_plane->GetPosition());
            DrawVector3Text("Rotation: ",10,30,current_plane->GetAngleDegree());
            DrawVector3Text("Velocity: ",10,50,current_plane->GetVelocity());
            DrawText(current_plane->name, 10, HEIGHT-30, 20, BLUE);
        window.EndDrawing();
    }
    current_plane = nullptr;
    return 0;
}

void inline DrawVector3Text(std::string title ,int posX, int posY, raylib::Vector3 vector){
    raylib::Text print("", 20 , WHITE, GetFontDefault(), 1.5);
    std::stringstream convert; 
    convert.str(std::string());
    convert<<title<<vector.x<<", "<<vector.y<<", "<<vector.z;
    print.text = convert.str();
    print.Draw(posX,posY);
}
void inline DrawControls(){
    DrawText("Use [W] and [S] to accelerate and decelerate", GetRenderWidth() - MeasureText("Use [W] and [S] to accelerate and decelerate",20) - 5, 10, 20, GREEN);
    DrawText("Use [A] and [D] to toggle turn left or turn right", GetRenderWidth() - MeasureText("Use [A] and [D] to turn left or turn right",20) - 5, 30, 20, GREEN);
    DrawText("Use [Q] and [E] to accelerate and decelerate vertically", GetRenderWidth() - MeasureText("Use [Q] and [E] to accelerate and decelerate vertically",20) - 5, 50, 20, GREEN);
    DrawText("Use ARROW KEYS to move camera up, down, left and right", GetRenderWidth() - MeasureText("Use ARROW KEYS to move camera up, down, left and right",20) - 5, 70, 20, GREEN);
    DrawText("Use TAB to switch between planes", GetRenderWidth() - MeasureText("Use TAB to switch between planes",20) - 5, 90, 20, GREEN);
}