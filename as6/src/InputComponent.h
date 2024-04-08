#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "Component.h"

struct InputComponent: public Component{
    bool enabled;
    using Component::Component;
    void setup() override{}
    void cleanup() override{}
    void update(float dt) override;
};

#endif 