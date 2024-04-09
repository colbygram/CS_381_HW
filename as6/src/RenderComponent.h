#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "Component.h"

struct RenderComponent : public Component{
    bool selected;
    raylib::Model* model;
    RenderComponent(struct Entity* en,raylib::Model* model);
    void setup() override{}
    void cleanup() override{}
    void update(float dt) override;
};

#endif