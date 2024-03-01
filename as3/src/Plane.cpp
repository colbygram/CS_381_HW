#include "Plane.h"

void Plane::CalculateForwardVector(){
    forward_vector.x = cos(angle_radian.x) * cos(angle_radian.y);
    forward_vector.y = -sin(angle_radian.x);
    forward_vector.z = cos(angle_radian.x) * -sin(angle_radian.y);
}
void Plane::HandleInput(){
    if(IsKeyPressed(KEY_W)){
        speed += (acceleration * GetFrameTime());
    }
    if(IsKeyPressed(KEY_S)){
        speed -= (acceleration * GetFrameTime());
    }
    if(IsKeyPressed(KEY_D) && !turn_right){
        turn_right = true;
        turn_left = false;
        rotation.y = (-turn_rate * GetFrameTime());
    }else if(IsKeyPressed(KEY_D) && turn_right){
        turn_right = false;
        rotation.y = 0;
    }
    if(IsKeyPressed(KEY_A) && !turn_left){
        turn_left = true;
        turn_right = false;
        rotation.y = (turn_rate * GetFrameTime());
    }else if(IsKeyPressed(KEY_A) && turn_left){
        turn_left = false;
        rotation.y = 0;
    }
    if(IsKeyPressed(KEY_Q)){
        vertical_speed += (acceleration * GetFrameTime());
    }
    if(IsKeyPressed(KEY_E)){
        vertical_speed -= (acceleration * GetFrameTime());
    }
}
void Plane::Update(){
    model->transform = transform;
    if(selected){
        HandleInput();
    }
    if(speed > max_speed){
        speed = max_speed;
    }else if(speed < -max_speed){
        speed = -max_speed;
    }
    angle_radian += rotation;
    CalculateForwardVector();
    velocity = (forward_vector.Normalize() * speed) + (raylib::Vector3(0,1,0) * vertical_speed);
    //Ground Check
    if((position.y + velocity.y) < 0){
        transform = raylib::Transform(model->transform).Translate(raylib::Vector3(0,-position.y,0));
        //model->SetTransform(raylib::Transform(model->transform).Translate(raylib::Vector3(0,-position.y,0)));
        position.y = 0;
        velocity.y = 0;
        vertical_speed = 0;
    }
    position += velocity;
    transform = raylib::Transform(model->transform).Translate(velocity).RotateXYZ(rotation);
    //model->SetTransform(raylib::Transform(model->transform).Translate(velocity).RotateXYZ(rotation));
}
void Plane::Draw(){
    raylib::Transform backupT = model->transform;
    model->SetTransform(transform);
    model->Draw({});
    if(selected){
        model->GetTransformedBoundingBox().Draw();
    }
    model->transform = backupT;
    // model->Draw({});
}
void Plane::SetSelection(const bool selection){
    selected = selection;
}
raylib::Vector3 Plane::GetPosition(){
    return position;
}
raylib::Vector3 Plane::GetAngleDegree(){
    return angle_radian*((180/PI));
}
raylib::Vector3 Plane::GetVelocity(){
    return velocity;
}
void Plane::SetNextPlane(Plane* plane){
    next_plane = plane;
}
Plane* Plane::GetNextPlane(){
    return next_plane;
}