#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "Component.h"

struct TransformComponent : public Component{
    using Component::Component;
    raylib::Vector3 position = {0,0,0};
    raylib::Quaternion rotation = raylib::Quaternion::Identity();
    void setup() override{}
    void cleanup() override{}
    void update(float dt) override{}
};
#endif