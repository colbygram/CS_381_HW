//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           magnitude to vector (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
//Problem: Render a plane model, skybox, and ground. Add controls to the plane to simulate plane behavior
//Solution: Use designated inputs, vector math and physics to allow the plane to move in a semi realistic behavior
//Extra Credit: I built the plane to more accurately move according to how a plane would move, but still left comments, explaining that I understand how to
//              move the plane in the default way. Also added arrow key movement to the camera, but it would operate the exact same as WASD if mapped to the plane.
//              They could be alternated and it would still work
#include <string>
#include <sstream>
#include "../raylib-cpp/include/raylib-cpp.hpp"

//Determine if the given vector's magnitude excedes the given max magnitude (specifically designed for the x and z axis)
bool ExcedeMaxForwardVel(raylib::Vector3 vel, const float max){
    float mag = sqrtf(powf(vel.x, 2) + powf(vel.z, 2));
    if(mag>max){
        return true;
    }else{
        return false;
    }
}

int main(){
    //Consts
    const int WIDTH = 1920/2;
    const int HEIGHT = 1080/2;
    const std::string mesh_path = "../assets/meshes/";
    const std::string texture_path = "../assets/textures/";
    const std::string plane_file = "PolyPlane.glb";
    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 2");
    raylib::Camera3D main_camera(raylib::Vector3(150,150,150), raylib::Vector3(0,0,0), raylib::Vector3(0,1,0), 90, CAMERA_PERSPECTIVE);
    //Model loaded
    raylib::Model plane;
    plane.Load(mesh_path + plane_file);
    //Vectors for different aspects of the plane
    raylib::Vector3 camera_position(250,250,250);
    raylib::Vector3 plane_position(0,0,0); 
    raylib::Vector3 plane_velocity(0,0,0);
    raylib::Vector3 plane_forward(1,0,0);
    raylib::Vector3 plane_rotation(0,0,0);
    raylib::Vector3 plane_current_angles_radian(0,0,0);
    raylib::Vector3 plane_current_angles_degrees(0,0,0);
    //floats to store different speeds and maxs
    float plane_speed = 2.5;
    float plane_turn_rate = 1;
    float camera_speed = 100;
    float lerp_speed = 5;
    float max_vel = 1;
    //Text object used to print out plane velocity, position and rotation
    raylib::Text print("", 20 , WHITE, GetFontDefault(), 1.5);
    std::stringstream convert; //Stringstream to help convert nums to strings. May be a more efficent way but this is the way i know of

    while (!window.ShouldClose())
    {
        raylib::Vector3 forward_sign;
        //plane input handling
        if(IsKeyDown(KEY_W)){
            forward_sign = plane_forward.Normalize();
            plane_velocity += (plane_forward.Normalize()) * (plane_speed * GetFrameTime());
        }
        if(IsKeyDown(KEY_S)){
            forward_sign = -plane_forward.Normalize();
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
        //update rotations and position of plane
        plane_current_angles_radian += plane_rotation;
        plane_position += plane_velocity;
        plane_current_angles_degrees = plane_current_angles_radian*(180/PI);

        //Calculate the forward of the plane according to its current rotation (+X is the local forward)
        plane_forward.x = cos(plane_current_angles_radian.x) * cos(plane_current_angles_radian.y);
        plane_forward.y = -sin(plane_current_angles_radian.x);
        plane_forward.z = cos(plane_current_angles_radian.x) * -sin(plane_current_angles_radian.y);
        
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
            ClearBackground(GRAY);
            convert.str(std::string());
            convert<<"Position: "<<plane_position.x<<", "<<plane_position.y<<", "<<plane_position.z;
            print.text = convert.str();
            print.Draw(10,10);
            convert.str(std::string());
            convert<<"Rotation: "<<plane_current_angles_degrees.x<<", "<<plane_current_angles_degrees.y<<", "<<plane_current_angles_degrees.z;
            print.text = convert.str();
            print.Draw(10,30);
            convert.str(std::string());
            convert<<"Velocity: "<<plane_velocity.x<<", "<<plane_velocity.y<<", "<<plane_velocity.z;
            print.text = convert.str();
            print.Draw(10,50);
            main_camera.BeginMode();
                DrawGrid(100, 25);
                plane.Draw(raylib::Vector3(0,0,0), 1.0, WHITE);
            main_camera.EndMode();
        EndDrawing();
    }
    return 0;
}