///@author: Colby Gramelspacher
///@ref: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
///      vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
///      forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles),
///      class examples (ComponentStorage implementation)
//Extra Credit: 

#include "Resources.hpp"

#define WIDTH 1024
#define HEIGHT 512

//Map 32x16 tiles
std::string map = "W..............................W"
                  "W..............................W"
                  "W..............................W"
                  "W.........FFFF.................E"
                  "W......................FFFFFFFFW"
                  "WFFFFFF........................W"
                  "W.............OOO..............W"
                  "W........FFFFFFFF..............W"
                  "W..............................W"
                  "W...............FFFFFF.........W"
                  "W......OO......................W"
                  "WFFFFFFFFFFFFF.................W"
                  "W....................OO........W"
                  "W...................FFFFFFFFFFFW"
                  "D.P............................W"
                  "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

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
struct Box{
    raylib::Rectangle m_rec;
    std::vector<Entity> entities;
};

void RenderSystem(SceneManager& scene){
    for(int e = 0; e < scene.entity_masks.size(); e++){
        if(!scene.HasComponent<Position>(e) && !scene.HasComponent<Tile>(e)) continue;
        auto& pos = scene.GetComponent<Position>(e);
        auto& tile = scene.GetComponent<Tile>(e);
        DrawRectangle(pos.m_position.x, pos.m_position.y, 32, 32, tile.m_color);
    }
}
void MoveSystem(SceneManager& scene, Entity player, raylib::BufferedInput& input, float speed, float dt){
    if(!scene.HasComponent<Position>(player) && !scene.HasComponent<CollisionBox>(player)) return;
    auto& pos = scene.GetComponent<Position>(player);
    auto& crec = scene.GetComponent<CollisionBox>(player);
    pos.m_position.x += speed * dt * (-input["Left"].data.button.last_state + input["Right"].data.button.last_state);
}
void PhysicsSystem(SceneManager& scene){
    
}

void CollisionSystem(SceneManager& scene){
    for(size_t e = 0; e < scene.entity_masks.size(); e++){
        if(!scene.HasComponent<CollisionBox>(e) || !scene.HasComponent<Tile>(e) || !scene.HasComponent<Position>(e)) continue;
        auto& crec = scene.GetComponent<CollisionBox>(e);
        auto& tile = scene.GetComponent<Tile>(e);
        auto& pos = scene.GetComponent<Position>(e);
        
    }
}

void SetupScene(SceneManager& scene, const std::string& map, Entity& player, std::vector<Box>& boxes){
    raylib::Vector2 current_read = {0,0};
    size_t row_check = 8;
    for(size_t i = 0; i < 32; i++){
        
    }
    current_read = raylib::Vector2::Zero();
    row_check = 32;
    for(char c: map){
        if(row_check <= 0){
            row_check = 32;
            current_read.x = 0;
            current_read.y += 32;
        }

        if(c == '.') {
            current_read.x += 32;
            row_check--;
            continue;    
        }
        
        Entity e = scene.CreateEntity();
        auto& pos = scene.AddComponent<Position>(e);
        auto& tile = scene.AddComponent<Tile>(e);
        auto& crec = scene.AddComponent<CollisionBox>(e);
        pos.m_position = current_read;
        crec.m_rec = raylib::Rectangle(pos.m_position.x, pos.m_position.y, 32, 32);
        current_read.x += 32;

        switch(c){
            case 'W':
                tile.current_tile = WALL;
                tile.m_color = DARKGRAY;
                break;
            case 'F':
                tile.current_tile = FLOOR;
                crec.m_rec.height = 8;
                tile.m_color = BROWN;
                break;
            case 'O':
                tile.current_tile = ENEMY;
                tile.m_color = RED;
                break;
            case 'D':
                tile.current_tile = DOOR;
                tile.m_color = BLACK;
                break;
            case 'P':
                tile.current_tile = PLAYER;
                player = e;
                tile.m_color = GREEN;
                break;
            case 'E':
                tile.current_tile = EXIT;
                tile.m_color = DARKGREEN;
                break;
            default:
                tile.current_tile = NONE;
                break;
        }
        row_check--;
    }
}

int main(){
    const std::string texture_path = "assets/textures/";
    
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 9");
    raylib::Camera2D main_camera(raylib::Vector2::Zero(), raylib::Vector2::Zero(), 0, 1);

    raylib::BufferedInput player_inputs;
    player_inputs["Left"] = raylib::Action::key(KEY_A).move();
    player_inputs["Right"] = raylib::Action::key(KEY_D).move();
    player_inputs["Jump"] = raylib::Action::key(KEY_SPACE).move();

    SceneManager scene;
    Entity player;
    SetupScene(scene, map, player);

    std::cout << scene.entity_masks.size() << std::endl;

    while (!window.ShouldClose()) {
        player_inputs.PollEvents();
        MoveSystem(scene, player, player_inputs, 100, GetFrameTime());

        window.BeginDrawing();
            window.ClearBackground(GRAY);
            main_camera.BeginMode();
                RenderSystem(scene);
            main_camera.EndMode();
            DrawFPS(10,10);
        window.EndDrawing();
    }
    return 0;
}