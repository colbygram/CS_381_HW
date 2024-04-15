//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
//Problem: 
//Solution: 
//Extra Credit: 

#include "Resources.h"

void VehicleControls(const bool toggle){
    if(toggle){
        DrawRectangle(0,0,GetRenderWidth(), GetRenderHeight(), raylib::Color(0,0,0,150));
        DrawText("PLANE", GetRenderWidth() - MeasureText("PLANE",20) - 5, 10, 20, RED);
        DrawText("Use [W] and [S] to accelerate and decelerate", GetRenderWidth() - MeasureText("Use [W] and [S] to accelerate and decelerate",20) - 5, 30, 20, GREEN);
        DrawText("Use ARROW KEYS to turn the plane and ascend/descend", GetRenderWidth() - MeasureText("Use ARROW KEYS to turn the plane and ascend/descend",20) - 5, 50, 20, GREEN);
        DrawText("Use TAB to switch between vehicles", GetRenderWidth() - MeasureText("Use TAB to switch between vehicles",20) - 5, 70, 20, GREEN);
        DrawText("BOAT", GetRenderWidth() - MeasureText("BOAT",20) - 5, 100, 20, RED);
        DrawText("Use [W] and [S] to accelerate and decelerate", GetRenderWidth() - MeasureText("Use [W] and [S] to accelerate and decelerate",20) - 5, 120, 20, GREEN);
        DrawText("Use [A] and [D] to turn the boats left and right", GetRenderWidth() - MeasureText("Use [A] and [D] to turn the boats left and right",20) - 5, 140, 20, GREEN);
        DrawText("Use TAB to switch between vehicles", GetRenderWidth() - MeasureText("Use TAB to switch between vehicles",20) - 5, 160, 20, GREEN);
    }else{
        DrawText("[C] for controls", GetRenderWidth() - MeasureText("[ESC] for controls",20) - 5, 10, 20, GREEN);
    }
}

int main(){
    //Consts
    const int WIDTH = 1920/2;
    const int HEIGHT = 1080/2;
    const int max_size = 10;
    const std::string mesh_path = "assets/meshes/";
    const std::string texture_path = "assets/textures/";
    const std::string plane_file = "PolyPlane.glb";
    const std::string boat_files[5] = {"SmitHouston_Tug.glb", "CargoG_HOSBrigadoon.glb", "Container_ShipLarge.glb", "OilTanker.glb", "OrientExplorer.glb"};
    const std::string water_file = "water.jpg";
    const std::string skybox_file = "skybox.png";
    
    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 6");
    raylib::Camera3D main_camera(raylib::Vector3(500,150,500), raylib::Vector3(0,0,0), raylib::Vector3(0,1,0), 90, CAMERA_PERSPECTIVE);
    raylib::Vector3 camera_offset = {0,50,-100};
    
    //water plane and texture load
    Mesh plane_mesh = raylib::Mesh::Plane(10'000, 10'000, 16, 16, 10);
    raylib::Model water_plane = ((raylib::Mesh*)&plane_mesh)->LoadModelFrom();
    raylib::Texture2D water(texture_path + water_file);
    water.SetFilter(TEXTURE_FILTER_BILINEAR);
    water.SetWrap(TEXTURE_WRAP_REPEAT);
    water_plane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = water;
    
    //Boat and Plane model load
    raylib::Model plane_model(mesh_path+plane_file);
    //Plane models default orientation needed to be updated by -90 degrees
    plane_model.SetTransform(raylib::Transform(plane_model.transform).RotateY(raylib::Radian(-1.571)));
    raylib::Model boat01_model(mesh_path+boat_files[0]), boat02_model(mesh_path+boat_files[1]), boat03_model(mesh_path+boat_files[2]), boat04_model(mesh_path+boat_files[3]), boat05_model(mesh_path+boat_files[4]);
    //each boat needs to be adjusted
    boat02_model.SetTransform(raylib::Transform(boat02_model.transform).RotateX(raylib::Radian(1.571)).RotateY(PI));
    boat03_model.SetTransform(raylib::Transform(boat03_model.transform).RotateX(raylib::Radian(1.571)));
    boat04_model.SetTransform(raylib::Transform(boat04_model.transform).RotateX(raylib::Radian(1.571)));
    boat05_model.SetTransform(raylib::Transform(boat05_model.transform).RotateX(raylib::Radian(1.571)).RotateY(PI));
    //Entity container
    std::vector<Entity*> entities;
    int current_index = 0;
    //Boat init and push_back into entity container
    Boat boat01(&boat01_model, 20, 1, 5), boat02(&boat02_model, 20, 1, 10), boat03(&boat03_model, 10, 0.25, 3), boat04(&boat04_model, 12, 0.25, 6), boat05(&boat05_model, 15, 1, 9);
    boat01.transform->position = raylib::Vector3(0,0, -150);
    boat02.transform->position = raylib::Vector3(0,0, -75);
    boat02.transform->scale = raylib::Vector3(0.01,0.01,0.01);
    boat03.transform->position = raylib::Vector3(0,0, 0);
    boat03.transform->scale = raylib::Vector3(0.01,0.01,0.01);
    boat04.transform->position = raylib::Vector3(0,0, 75);
    boat04.transform->scale = raylib::Vector3(0.01,0.01,0.01);
    boat05.transform->position = raylib::Vector3(0,0, 150);
    boat05.transform->scale = raylib::Vector3(0.01,0.01,0.01);
    entities.push_back(&boat01);
    entities.push_back(&boat02);
    entities.push_back(&boat03);
    entities.push_back(&boat04);
    entities.push_back(&boat05);
    //Plane init and push_back into entity container
    Plane plane01(&plane_model), plane02(&plane_model), plane03(&plane_model), plane04(&plane_model), plane05(&plane_model);
    plane01.transform->position = raylib::Vector3(0,50, -150);
    plane02.transform->position = raylib::Vector3(0,50, -75);
    plane03.transform->position = raylib::Vector3(0,50, 0);
    plane04.transform->position = raylib::Vector3(0,50, 75);
    plane05.transform->position = raylib::Vector3(0,50, 150);
    entities.push_back(&plane01);
    entities.push_back(&plane02);
    entities.push_back(&plane03);
    entities.push_back(&plane04);
    entities.push_back(&plane05);
    
    //Control menu toggle
    bool control_toggle = false;

    //setup entities and components
    for(auto& e: entities){
        e->setup();
    }
    entities[current_index]->Select();
    
    //Game loop
    while (!window.ShouldClose()) {
        if(IsKeyPressed(KEY_C)){
            control_toggle = !control_toggle;
        }
        //Iterate through entities
        if(IsKeyPressed(KEY_TAB)){
            entities[current_index]->Deselect();
            current_index = (current_index+1) % max_size;
            entities[current_index]->Select();
        }
        main_camera.SetPosition(entities[current_index]->transform->position + camera_offset);
        main_camera.SetTarget(entities[current_index]->transform->position);
        //Render
        window.BeginDrawing();
            window.ClearBackground(GRAY);
            main_camera.BeginMode();
                water_plane.Draw({});
                for(auto& e: entities){
                    e->update(GetFrameTime());
                }
            main_camera.EndMode();
            VehicleControls(control_toggle);
        window.EndDrawing();
    }
    //cleanup entities and components
    for(auto& e: entities){
        e->cleanup();
    }
    return 0;
}