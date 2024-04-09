#ifndef VEHICLES_H
#define VEHICLES_H

#include "Entity.h"

struct Vehicle: public Entity{
    Vehicle(raylib::Model* model);
};

struct Plane : public Vehicle{
    Plane(raylib::Model* model);
    void setup() override;
    void update(float dt) override;
};

struct Boat : public Vehicle{
    using Vehicle::Vehicle;
    PhysicComponent* physics;
    InputComponent* input_manager;
    TransformComponent* transform;
    void setup() override;
    void update(float dt) override;
};
#endif