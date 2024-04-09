//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
//Problem: 
//Solution: 
//Extra Credit: 

#include "Resources.h"

void inline DrawVector3Text(std::string title ,int posX, int posY, raylib::Vector3 vector);
void inline DrawControls();

int main(){
    //Consts
    const int WIDTH = 1920/2;
    const int HEIGHT = 1080/2;
    const std::string mesh_path = "assets/meshes/";
    const std::string texture_path = "assets/textures/";
    const std::string plane_file = "PolyPlane.glb";
    const std::string boat_file = "SmitHouston_Tug.glb";
    const std::string water_file = "water.jpg";
    const std::string skybox_file = "skybox.png";
    
    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 6");
    raylib::Camera3D main_camera(raylib::Vector3(500,150,500), raylib::Vector3(0,0,0), raylib::Vector3(0,1,0), 90, CAMERA_PERSPECTIVE);
    
    //water plane and texture load
    auto plane_mesh = raylib::Mesh::Plane(10'000, 10'000, 16, 16, 10);
    raylib::Model water_plane = ((raylib::Mesh*)&plane_mesh)->LoadModelFrom();
    raylib::Texture2D water(texture_path + water_file);
    water.SetFilter(TEXTURE_FILTER_BILINEAR);
    water.SetWrap(TEXTURE_WRAP_REPEAT);
    water_plane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = water;
    
    //Boat and Plane model load
    raylib::Model plane_model(mesh_path+plane_file);
    raylib::Model boat_model(mesh_path+boat_file);

    std::vector<Entity*> entities;
    Boat boat01(&boat_model);
    entities.push_back(&boat01);

    main_camera.SetTarget(boat01.GetComponent<TransformComponent>()->get().position);
    raylib::Vector3 camera_offset = {0,25,-50};
    //setup entities and components
    for(auto& e: entities){
        e->setup();
    }
    boat01.Select();
    while (!window.ShouldClose()) {
        main_camera.SetPosition(boat01.GetComponent<TransformComponent>()->get().position + camera_offset);
        main_camera.SetTarget(boat01.GetComponent<TransformComponent>()->get().position);
        if(IsKeyPressed(KEY_TAB)){
            if(boat01.selected){
                boat01.Deselect();
            }else{
                boat01.Select();
            }
        }
        //Render
        window.BeginDrawing();
            window.ClearBackground(GRAY);
            main_camera.BeginMode();
                water_plane.Draw({});
                for(auto& e: entities){
                    e->update(GetFrameTime());
                }
            main_camera.EndMode();
            DrawControls();
        window.EndDrawing();
    }
    //cleanup entities and components
    for(auto& e: entities){
        e->cleanup();
    }

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