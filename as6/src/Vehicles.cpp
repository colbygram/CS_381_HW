#include "Vehicles.h"
#include "TransformComponent.h"
#include "PhysicComponent.h"
#include "RenderComponent.h"
#include "InputComponent.h"

////////////////////////////////////VEHICLE STRUCT////////////////////////////////////////////////
Vehicle::Vehicle(raylib::Model* model) : Entity(){
    AddComponent<RenderComponent>(model);
    AddComponent<InputComponent>();
    AddComponent<PhysicComponent>();
}

////////////////////////////////////PLANE STRUCT////////////////////////////////////////////////
void Plane::setup(){
    {
        auto input_ref = GetComponent<InputComponent>();
        if(!input_ref) return;
        input_manager = &input_ref->get();
        auto phys_ref = GetComponent<PhysicComponent>();
        if(!phys_ref) return;
        physics = &phys_ref->get();
    }
    input_manager->inputs["W"] = raylib::Action::key(KEY_W)
    .SetPressedCallback([this]{
        physics->IncrementSpeed();
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
void Boat::setup(){
    {
        auto input_ref = GetComponent<InputComponent>();
        if(!input_ref) return;
        input_manager = &input_ref->get();
        auto phys_ref = GetComponent<PhysicComponent>();
        if(!phys_ref) return;
        physics = &phys_ref->get();
    }
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