#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Component.h"

struct TransformComponent : public Component{
    using Component::Component;
    raylib::Vector3 position = {0,0,0};
    raylib::Vector3 euler_angles = {0,0,0};
    raylib::Vector3 scale = {1,1,1};
    raylib::Quaternion rotation = raylib::Quaternion::FromEuler(euler_angles);
    void setup() override{}
    void cleanup() override{}
    void update(float dt) override{
        rotation = raylib::Quaternion::FromEuler(euler_angles);
    }
};
#endif