#include "Vehicles.h"
#include "TransformComponent.h"
#include "PhysicComponent.h"
#include "RenderComponent.h"
#include "InputComponent.h"
#include <iostream>

////////////////////////////////////VEHICLE STRUCT////////////////////////////////////////////////
Vehicle::Vehicle(raylib::Model* model) : Entity(){
    AddComponent<RenderComponent>(model);
    AddComponent<InputComponent>();
    AddComponent<PhysicComponent>();
}

////////////////////////////////////PLANE STRUCT////////////////////////////////////////////////
Plane::Plane(raylib::Model* model): Vehicle(model){
    auto input_ref = GetComponent<InputComponent>();
    if(!input_ref) return;
    input_manager = &input_ref->get();
    auto phys_ref = GetComponent<PhysicComponent>();
    if(!phys_ref) return;
    physics = &phys_ref->get();
}
void Plane::setup(){
    input_manager->inputs["W"] = raylib::Action::key(KEY_W)
    .SetPressedCallback([this]{
        physics->IncrementSpeed();
        std::cout << "W" << std::endl;
    }).move();
    input_manager->inputs["S"] = raylib::Action::key(KEY_S)
    .SetPressedCallback([this]{
        physics->DecrementSpeed();
    }).move();
    input_manager->inputs["Left"] = raylib::Action::key(KEY_LEFT)
    .SetPressedCallback([this]{
        physics->IncrementYaw();
    }).move();
    input_manager->inputs["Right"] = raylib::Action::key(KEY_RIGHT)
    .SetPressedCallback([this]{
        physics->DecrementYaw();
    }).move();
    input_manager->inputs["Up"] = raylib::Action::key(KEY_UP)
    .SetPressedCallback([this]{
        physics->IncrementPitch();
    }).move();
    input_manager->inputs["Down"] = raylib::Action::key(KEY_DOWN)
    .SetPressedCallback([this]{
        physics->DecrementPitch();
    }).move();
    input_manager->inputs["SPACE"] = raylib::Action::key(KEY_SPACE)
    .SetPressedCallback([this]{
        physics->Reset();
    }).move();

    for(auto& c : components){
        c->setup();
    }
}

////////////////////////////////////BOAT STRUCT////////////////////////////////////////////////
Boat::Boat(raylib::Model* model, float acceleration, float turn_rate, float max_speed): Vehicle(model){
    auto input_ref = GetComponent<InputComponent>();
    if(!input_ref) return;
    input_manager = &input_ref->get();
    auto phys_ref = GetComponent<PhysicComponent>();
    if(!phys_ref) return;
    physics = &phys_ref->get();

    physics->acceleration = acceleration;
    physics->turn_rate = turn_rate;
    physics->max_speed = max_speed;
}
void Boat::setup(){
    input_manager->inputs["W"] = raylib::Action::key(KEY_W)
    .SetPressedCallback([this]{
        physics->IncrementSpeed();
    }).move();
    input_manager->inputs["S"] = raylib::Action::key(KEY_S)
    .SetPressedCallback([this]{
        physics->DecrementSpeed();
    }).move();
    input_manager->inputs["A"] = raylib::Action::key(KEY_A)
    .SetPressedCallback([this]{
        physics->IncrementYaw();
    }).move();
    input_manager->inputs["D"] = raylib::Action::key(KEY_D)
    .SetPressedCallback([this]{
        physics->DecrementYaw();
    }).move();
    input_manager->inputs["SPACE"] = raylib::Action::key(KEY_SPACE)
    .SetPressedCallback([this]{
        physics->Reset();
    }).move();

    for(auto& c : components){
        c->setup();
    }
}