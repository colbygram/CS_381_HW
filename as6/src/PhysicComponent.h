#ifndef PHYSICCOMPONENT_H
#define PHYSICCOMPONENT_H

#include "Component.h"

struct PhysicComponent: public Component{
    raylib::Vector3 velocity;
    float speed, acceleration;
    void setup() override;
    void update(float dt) override;
    void cleanup() override{}
};

#endif