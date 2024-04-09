#ifndef PHYSICCOMPONENT_H
#define PHYSICCOMPONENT_H

#include "Component.h"

struct PhysicComponent: public Component{
    using Component::Component;
    raylib::Vector3 velocity;
    int turn_dir_yaw, turn_dir_pitch;
    float speed, acceleration, max_speed, turn_rate, dt;
    raylib::Vector3 forward_vector;
    void setup() override;
    void update(float dt) override;
    void cleanup() override{}

    //Utilties
    //Increases speed toward forward_vector
    void IncrementSpeed();
    //Decreases speed on forward_vector
    void DecrementSpeed();
    //Resets speed to 0
    void ResetSpeed();

    //Calculate forward vector
    void CalculateForwardVector(raylib::Vector3 eulers);

    //Control Yaw axis turn direction
    void IncrementYaw();
    void DecrementYaw();
    //Control Pitch 
    void IncrementPitch();
    void DecrementPitch();
    //Reset
    void Reset();
};

#endif