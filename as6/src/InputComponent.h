#ifndef INPUTCOMPONENT_H
#define INPUTCOMPONENT_H

#include "Component.h"
#include <BufferedInput.hpp>

//InputComponent will use W and S to control forward and backward motions. When W is pressed, move_direction is incremented by 1, when S is pressed, it's decremented 1
//Turning used bools to determine if the object is turning either left(A) or right(D).
struct InputComponent: public Component{
    using Component::Component;
    bool enabled;
    raylib::BufferedInput inputs;
    void setup() override;
    void cleanup() override;
    void update(float dt) override;
};

#endif 