#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component.h"

struct RenderComponent : public Component{
    raylib::Model* model;
    using Component::Component;
    void setup() override{}
    void cleanup() override{}
    void update(float dt) override;
};

#endif