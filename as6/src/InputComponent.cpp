#include "InputComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include <iostream>

void InputComponent::setup(){
    enabled = false;
}

void InputComponent::update(float dt){
    if(enabled){
        inputs.PollEvents();
    }
}

void InputComponent::cleanup(){
    inputs.actions.clear();
}