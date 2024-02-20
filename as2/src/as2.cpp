//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           magnitude to vector (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
//Problem: Render a plane model, skybox, and ground. Add controls to the plane to simulate plane behavior
//Solution: Use designated inputs, vector math and physics to allow the plane to move in a semi realistic behavior
//Extra Credit: I built the plane to more accurately move according to how a plane would move, but still left comments, explaining that I understand how to
//              move the plane in the default way. Also added arrow key movement to the camera, but it would operate the exact same as WASD if mapped to the plane.
//              They could be alternated and it would still work

#include "Resources.h"

//Determine if the given vector's magnitude excedes the given max magnitude (specifically designed for the x and z axis)
bool inline ExcedeMaxForwardVel(raylib::Vector3 vel, const float max);
void inline DrawVector3Text(std::string title ,int posX, int posY, raylib::Vector3 vector);
//Calculate the forward of the plane according to its current rotation (+X is the local forward)
void inline CalculateForwardVector(raylib::Vector3& forward, raylib::Vector3 rotation);

int main(){
    //Consts
    const int WIDTH = 1920/2;
    const int HEIGHT = 1080/2;
    const std::string mesh_path = "../assets/meshes/";
    const std::string texture_path = "../assets/textures/";
    const std::string plane_file = "PolyPlane.glb";
    const std::string grass_file = "grass.jpg";
    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 2");
    raylib::Camera3D main_camera(raylib::Vector3(150,150,150), raylib::Vector3(0,0,0), raylib::Vector3(0,1,0), 90, CAMERA_PERSPECTIVE);
    //Model loaded
    auto plane_mesh = raylib::Mesh::Plane(10'000, 10'000, 50, 50, 25);
    raylib::Model ground_plane = ((raylib::Mesh*)&plane_mesh)->LoadModelFrom();
    raylib::Texture2D grass(texture_path + grass_file);
    grass.SetFilter(TEXTURE_FILTER_BILINEAR);
    grass.SetWrap(TEXTURE_WRAP_REPEAT);
    ground_plane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = grass;
    raylib::Model plane(mesh_path + plane_file);
    //Vectors for different aspects of the plane
    raylib::Vector3 camera_position(250,250,250);
    raylib::Vector3 plane_position(0,0,0); 
    raylib::Vector3 plane_velocity(0,0,0);
    raylib::Vector3 plane_forward(1,0,0);
    raylib::Vector3 plane_rotation(0,0,0);
    raylib::Vector3 plane_current_angles_radian(0,0,0);
    raylib::Vector3 plane_current_angles_degrees(0,0,0);
    raylib::Ray plane_ray(plane_position, raylib::Vector3(0,-1,0));
    raylib::BoundingBox ground(raylib::Vector3(-1000,-100,-1000),raylib::Vector3(1000,0,1000));
    raylib::Color ray_color;
    //floats to store different speeds and maxs
    const float gravity = 9.8;
    float plane_mass = 100;
    float plane_speed = 2.5;
    float plane_turn_rate = 1;
    float camera_speed = 100;
    float lerp_speed = 5;
    float max_vel = 1;
    float jump_vel = 3;

    while (!window.ShouldClose())
    { 
        raylib::Vector3 forward_sign;
        //plane input handling
        if(IsKeyDown(KEY_W)){
            forward_sign = plane_forward;
            plane_velocity += (plane_forward.Normalize()) * (plane_speed * GetFrameTime());
        }
        if(IsKeyDown(KEY_S)){
            forward_sign = -plane_forward;
            plane_velocity += (-plane_forward.Normalize()) * (plane_speed * GetFrameTime());
        }
        if(!IsKeyDown(KEY_S) && !IsKeyDown(KEY_W)){
            float blend = 1 - powf(0.5, GetFrameTime() * lerp_speed);
            plane_velocity = Vector3Lerp(plane_velocity, raylib::Vector3(0,0,0), blend);
        }
        if(IsKeyDown(KEY_D)){
            plane_rotation.y = (-plane_turn_rate * GetFrameTime());
        }else if(!IsKeyDown(KEY_A)){
            plane_rotation.y = 0;
        }
        if(IsKeyDown(KEY_A)){
            plane_rotation.y = (plane_turn_rate * GetFrameTime());
        }else if(!IsKeyDown(KEY_D)){
            plane_rotation.y = 0;
        }
        
        CalculateForwardVector(plane_forward, plane_current_angles_radian);

        //update rotations and position of plane
        plane_current_angles_radian += plane_rotation;
        plane_position += plane_velocity;
        plane_current_angles_degrees = plane_current_angles_radian*(180/PI);

        plane_ray.position = plane_position;

        //Clamp magnitude of x and z axis of velocity if it excedes chosen max magnitude
        if(ExcedeMaxForwardVel(plane_velocity, max_vel)){
            plane_velocity.x = max_vel * forward_sign.Normalize().x;
            plane_velocity.z = max_vel * forward_sign.Normalize().z;
        }

        plane.SetTransform(raylib::Transform(plane.transform).Translate(plane_velocity).RotateXYZ(plane_rotation));

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
        main_camera.SetPosition(camera_position + plane_position);
        main_camera.SetTarget(plane_position);

        //Render
        BeginDrawing();
            window.ClearBackground(GRAY);
            DrawVector3Text("Position: ",10,10,plane_position);
            DrawVector3Text("Rotation: ",10,30,plane_current_angles_degrees);
            DrawVector3Text("Velocity: ",10,50,plane_velocity);
            main_camera.BeginMode();
                ground_plane.Draw({});
                plane_ray.Draw(RED);
                plane.Draw({});
                plane.GetTransformedBoundingBox().Draw();
            main_camera.EndMode();
        EndDrawing();
    }
    return 0;
}

//Determine if the given vector's magnitude excedes the given max magnitude (specifically designed for the x and z axis)
bool inline ExcedeMaxForwardVel(raylib::Vector3 vel, const float max){
    float mag = sqrtf(powf(vel.x, 2) + powf(vel.z, 2));
    if(mag>max){
        return true;
    }else{
        return false;
    }
}
void inline DrawVector3Text(std::string title ,int posX, int posY, raylib::Vector3 vector){
    raylib::Text print("", 20 , WHITE, GetFontDefault(), 1.5);
    std::stringstream convert; 
    convert.str(std::string());
    convert<<title<<vector.x<<", "<<vector.y<<", "<<vector.z;
    print.text = convert.str();
    print.Draw(posX,posY);
}
void inline CalculateForwardVector(raylib::Vector3& forward, raylib::Vector3 rotation){
    forward.x = cos(rotation.x) * cos(rotation.y);
    forward.y = -sin(rotation.x);
    forward.z = cos(rotation.x) * -sin(rotation.y);
}