#ifndef PLANE_H
#define PLANE_H

#include "raylib-cpp.hpp"

class Plane {   
private:
    raylib::Model* model;
    raylib::Vector3 position, velocity;
    float speed, vertical_speed, acceleration, increment;
    raylib::Transform transform;
    void HandleInput();

public:
    float max_speed;
    bool started, reset;

    Plane(raylib::Model* plane_model) : 
    position(raylib::Vector3(0,0,0))
    , velocity(raylib::Vector3(0,0,0))
    , speed(0.5)
    , max_speed(0.5)
    , vertical_speed(0)
    , acceleration(0.75)
    , started(false)
    , reset(false)
    , increment(0)
    , model(plane_model)
    , transform(model->transform){}

    void Update();
    void Draw();

    raylib::Vector3 GetPosition();
    bool CheckCollision(raylib::Model* box);
};

#endif