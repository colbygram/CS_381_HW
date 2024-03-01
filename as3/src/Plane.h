#ifndef PLANE_H
#define PLANE_H

#include "raylib-cpp.hpp"

class Plane {   
private:
    raylib::Model* model;
    raylib::Vector3 position, velocity, forward_vector, rotation, angle_radian;
    float speed, vertical_speed, turn_rate, acceleration;
    bool turn_left, turn_right, selected;
    raylib::Transform transform;
    Plane* next_plane;
    void CalculateForwardVector();
    void HandleInput();

public:
    float max_speed;
    const char* name;

    Plane(raylib::Model* plane_model) : 
    position(raylib::Vector3(0,0,0))
    , velocity(raylib::Vector3(0,0,0))
    , forward_vector(raylib::Vector3(1,0,0))
    , rotation(raylib::Vector3(0,0,0))
    , angle_radian(raylib::Vector3(0,0,0))
    , speed(0)
    , max_speed(5)
    , vertical_speed(0)
    , turn_rate(1)
    , acceleration(20)
    , model(plane_model)
    , turn_left(false)
    , turn_right(false)
    , selected(false)
    , transform(model->transform)
    , next_plane(nullptr){}

    void Update();
    void Draw();

    void SetSelection(const bool selection);
    raylib::Vector3 GetPosition();
    raylib::Vector3 GetAngleDegree();
    raylib::Vector3 GetVelocity();
    void SetNextPlane(Plane* plane);
    Plane* GetNextPlane();
};

#endif