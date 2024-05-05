#ifndef SYSTEM_HPP
#define SYSTEM_HPP
#include "Globals.hpp"
#include "DOECS.hpp"

struct System {
    static void RenderSystem(SceneManager& scene, const raylib::Rectangle& screen);
    static void MoveSystem(raylib::BufferedInput& input, 
                           struct State& player_state,
                           struct Position& player_pos);
    static void PhysicsSystem(struct Physics& phys, 
                              struct Position& pos, 
                              struct CollisionBox& cbox, 
                              struct State& state,  
                              float& jump_timer, 
                              float dt);
    static void CollisionSystem(SceneManager& scene, 
                                Entity player,
                                Entity spawn,
                                struct Position& player_pos, 
                                struct CollisionBox& player_cbox, 
                                struct State& player_state, 
                                struct Physics& player_phys);
    static void CameraSystem(struct Position& player_pos, raylib::Camera2D& camera, raylib::Rectangle& screen);
};

#endif