#ifndef VEHICLES_H
#define VEHICLES_H

#include "Entity.h"

struct Vehicle: public Entity{
    bool selected;
    Vehicle(raylib::Model* model);
};

struct Plane : public Vehicle{
    Plane(raylib::Model* model);
};

struct Boat : public Vehicle{
    Boat(raylib::Model* model);
};
#endif