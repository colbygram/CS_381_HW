#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <raylib-cpp.hpp>
#include <vector>

enum TileType{
    NONE = 0, FLOOR, WALL, ENEMY, PLAYER, DOOR, EXIT, SPAWN
};
struct Position{
    raylib::Vector2 m_position;
};
struct Tile{
    TileType current_tile;
    raylib::Texture2D* texture;
    raylib::Color m_color;
};
struct CollisionBox{
    raylib::Rectangle m_rec;
};
struct Physics{
    raylib::Vector2 velocity;
    float jump_speed;
    float max_speed;
};
struct State{
    bool is_grounded;
    bool is_jumping;
    int move_direction;
};
#endif