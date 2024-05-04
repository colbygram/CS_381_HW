#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include "Globals.hpp"
#include "DOECS.hpp"

struct System {
    static void RenderSystem(SceneManager& scene, const raylib::Rectangle& screen);
    static void MoveSystem(SceneManager& scene, 
                           Entity player, 
                           raylib::Camera2D& camera, 
                           raylib::BufferedInput& input, 
                           raylib::Rectangle& screen, 
                           float speed, float dt);
    static void PhysicsSystem(SceneManager& scene, Entity Player, bool& grounded, float gravity, float dt);
    static void CollisionSystem(SceneManager& scene, Entity player, bool& grounded);
};

#endif