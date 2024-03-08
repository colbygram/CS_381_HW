#include "Plane.h"

void Plane::HandleInput(){
    if(IsKeyDown(KEY_SPACE)){
        vertical_speed += (acceleration * GetFrameTime());
    }else{
        vertical_speed -= (acceleration * GetFrameTime());
    }
}
void Plane::Update(){
    model->transform = transform;

    if(reset){
        model->transform = raylib::Transform(model->transform).Translate(raylib::Vector3(-position.x,-position.y,0));
        reset = false;
        started = false;
        position.x = 0;
        position.y = 0;
    }

    HandleInput();

    if(position.x >= 1000){
        increment = 0.5;
    }else if(position.x >= 2000){
        increment = 1;
    }else if(position.x >= 4000){
        increment = 1.5;
    }else if(position.x >= 8000){
        increment = 2;
    }else if(position.x >= 16000){
        increment = 2.5;
    }else if(position.x >= 32000){
        increment = 3;
    }else{
        increment = 0;
    }

    if(started){
        speed = 0.5 + increment;
    }else{
        speed = 0;
    }

    if(vertical_speed > max_speed){
        vertical_speed = max_speed;
    }else if(vertical_speed < -max_speed){
        vertical_speed = -max_speed;
    }

    velocity = (raylib::Vector3(1,0,0) * speed) + (raylib::Vector3(0,1,0) * vertical_speed);
    
    //Ground Check
    if((position.y + velocity.y) < 0){
        transform = raylib::Transform(model->transform).Translate(raylib::Vector3(0,-position.y,0));
        position.y = 0;
        velocity.y = 0;
        vertical_speed = 0;
    }
    position += velocity;
    transform = raylib::Transform(model->transform).Translate(velocity);
}
void Plane::Draw(){
    raylib::Transform backupT = model->transform;
    model->SetTransform(transform);
    model->Draw({});
    model->transform = backupT;
}
raylib::Vector3 Plane::GetPosition(){
    return position;
}
bool Plane::CheckCollision(raylib::Model* box){
    if(CheckCollisionBoxes(model->GetTransformedBoundingBox(), box->GetTransformedBoundingBox())){
        return true;
    }
    return false;
}