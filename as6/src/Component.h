#ifndef COMPONENT_H
#define COMPONENT_H

#include "Resources.h"

struct Component{
    struct Entity* object;
    Component(struct Entity* en) : object(en){}
    virtual void setup() = 0;
    virtual void cleanup() = 0;
    virtual void update(float dt) = 0;
};
#endif