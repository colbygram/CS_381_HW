#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <raylib-cpp.hpp>
#include <vector>

enum TileType{
    NONE = 0, FLOOR, WALL, ENEMY, PLAYER, DOOR, EXIT
};
struct Position{
    raylib::Vector2 m_position;
};
struct Tile{
    TileType current_tile;
    raylib::Color m_color;
};
struct CollisionBox{
    raylib::Rectangle m_rec;
};

#endif