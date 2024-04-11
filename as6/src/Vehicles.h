#ifndef VEHICLES_H
#define VEHICLES_H

#include "Entity.h"

struct Vehicle: public Entity{
    PhysicComponent* physics;
    InputComponent* input_manager;

    Vehicle(raylib::Model* model);
};

struct Plane : public Vehicle{
    using Vehicle::Vehicle;
    void setup() override;
};

struct Boat : public Vehicle{
    using Vehicle::Vehicle;
    void setup() override;
};
#endif