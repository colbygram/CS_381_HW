#include "Vehicles.h"
#include "RenderComponent.h"
#include "InputComponent.h"

Vehicle::Vehicle(raylib::Model* model) : Entity(){
    selected = false;
    AddComponent<RenderComponent>(model);
    AddComponent<InputComponent>();
}

Plane::Plane(raylib::Model* model): Vehicle(model){
    
}

Boat::Boat(raylib::Model* model): Vehicle(model){
    
}