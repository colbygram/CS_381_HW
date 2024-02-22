//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
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
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 2");
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
    //Plane model loaded
    raylib::Model plane(mesh_path + plane_file);
    //Vectors for different aspects of the plane
    raylib::Vector3 camera_position(100,100,100);
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
    float plane_speed = 0;
    float plane_vertical_speed = 0;
    float plane_acceleration = 20;
    float plane_turn_rate = 1;
    float camera_speed = 100;
    float lerp_speed = 5;
    float max_vel = 1;
    float jump_vel = 3;
    bool turn_left = false, turn_right = false;

    while (!window.ShouldClose())
    { 
        //Plane Input Handling
        //Use velocity equation vf = vi + at where vi is initial velocity and vf is the next velocity
        //vf = plane_speed, a = plane_acceleration, t = frame time, vi is plane_speed from previous frame
        if(IsKeyPressed(KEY_W)){
            plane_speed += (plane_acceleration * GetFrameTime());
        }
        if(IsKeyPressed(KEY_S)){
            plane_speed -= (plane_acceleration * GetFrameTime());
        }
        if(IsKeyPressed(KEY_D) && !turn_right){
            turn_right = true;
            turn_left = false;
            plane_rotation.y = (-plane_turn_rate * GetFrameTime());
        }else if(IsKeyPressed(KEY_D) && turn_right){
            turn_right = false;
            plane_rotation.y = 0;
        }
        if(IsKeyPressed(KEY_A) && !turn_left){
            turn_left = true;
            turn_right = false;
            plane_rotation.y = (plane_turn_rate * GetFrameTime());
        }else if(IsKeyPressed(KEY_A) && turn_left){
            turn_left = false;
            plane_rotation.y = 0;
        }
        if(IsKeyPressed(KEY_Q)){
            plane_vertical_speed += (plane_acceleration * GetFrameTime());
        }
        if(IsKeyPressed(KEY_E)){
            plane_vertical_speed -= (plane_acceleration * GetFrameTime());
        }

        //Get rotation updates and calculate forward
        plane_current_angles_radian += plane_rotation;
        plane_current_angles_degrees = plane_current_angles_radian*(180/PI);
        CalculateForwardVector(plane_forward, plane_current_angles_radian);

        //Checks if the magnitude of plane_velocity.x and plane_velocity.z have exceeded set max_vel
        if(ExcedeMaxForwardVel(plane_velocity, max_vel)){
            plane_velocity = (plane_forward.Normalize() * max_vel) + (raylib::Vector3(0,1,0) * plane_vertical_speed);
        }else{
            plane_velocity = (plane_forward.Normalize() * plane_speed) + (raylib::Vector3(0,1,0) * plane_vertical_speed);
        }

        //Ground collision
        if((plane_position.y + plane_velocity.y) < 0){
            //When plane gets near ground, translate it to the ground and reset variables
            plane.SetTransform(raylib::Transform(plane.transform).Translate(raylib::Vector3(0,-plane_position.y,0)));    
            plane_position.y = 0;
            plane_velocity.y = 0;
            plane_vertical_speed = 0;
        }
        //Update position and transform plane
        plane_position += plane_velocity;
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
        window.BeginDrawing();
            window.ClearBackground(GRAY);
            DrawVector3Text("Position: ",10,10,plane_position);
            DrawVector3Text("Rotation: ",10,30,plane_current_angles_degrees);
            DrawVector3Text("Velocity: ",10,50,plane_velocity);
            DrawControls();
            main_camera.BeginMode();
                skybox.Draw();
                ground_plane.Draw({});
                plane.Draw({});
            main_camera.EndMode();
        window.EndDrawing();
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
void inline DrawControls(){
    DrawText("Use [W] and [S] to accelerate and decelerate", GetRenderWidth() - MeasureText("Use [W] and [S] to accelerate and decelerate",20) - 5, 10, 20, GREEN);
    DrawText("Use [A] and [D] to toggle turn left or turn right", GetRenderWidth() - MeasureText("Use [A] and [D] to turn left or turn right",20) - 5, 30, 20, GREEN);
    DrawText("Use [Q] and [E] to accelerate and decelerate vertically", GetRenderWidth() - MeasureText("Use [Q] and [E] to accelerate and decelerate vertically",20) - 5, 50, 20, GREEN);
    DrawText("Use ARROW KEYS to move camera up, down, left and right", GetRenderWidth() - MeasureText("Use ARROW KEYS to move camera up, down, left and right",20) - 5, 70, 20, GREEN);
}