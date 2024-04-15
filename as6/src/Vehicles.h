#ifndef VEHICLES_H
#define VEHICLES_H

#include "Entity.h"

struct Vehicle: public Entity{
    PhysicComponent* physics;
    InputComponent* input_manager;

    Vehicle(raylib::Model* model);
};

struct Plane : public Vehicle{
    Plane(raylib::Model* model);
    void setup() override;
};

struct Boat : public Vehicle{
    Boat(raylib::Model* model, float acceleration, float turn_rate, float max_speed);
    void setup() override;
};
#endif