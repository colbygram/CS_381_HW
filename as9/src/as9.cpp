///@author: Colby Gramelspacher
///@ref: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
///      vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
///      forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles),
///      class examples (ComponentStorage implementation)
//Extra Credit: 

#include "Resources.hpp"

//Map 48x16 tiles
std::string map = "W..............................................W"
                  "W..............................................W"
                  "W..............................................W"
                  "W.........FFFF.................................E"
                  "W......................FFFFFFFFFFFFFFFFF.......W"
                  "WFFFFFF........................................W"
                  "W.............OOO..............................W"
                  "W........FFFFFFFF...................FFFFFF.....W"
                  "W..............................................W"
                  "W...............FFFFFF.........................W"
                  "W......OO......................................W"
                  "WFFFFFFFFFFFFF........................FFFFF....W"
                  "W....................OO........................W"
                  "W...................FFFFFFFFFFF................W"
                  "D.P............................................W"
                  "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";


void SetupScene(SceneManager& scene, const std::string& map, Entity& player){
    raylib::Vector2 current_read = {0,0};
    size_t row_check = MAX_LEVEL_COLUMNS;
    for(char c: map){
        if(row_check <= 0){
            row_check = MAX_LEVEL_COLUMNS;
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
    raylib::Rectangle screen_rec(0,0, WIDTH, HEIGHT);
    bool grounded = true;

    while (!window.ShouldClose()) {
        player_inputs.PollEvents();
        System::MoveSystem(scene, player, main_camera, player_inputs, screen_rec, 100, GetFrameTime());
        System::PhysicsSystem(scene , player, grounded, 1, GetFrameTime());
        System::CollisionSystem(scene, player, grounded);
        window.BeginDrawing();
            window.ClearBackground(GRAY);
            main_camera.BeginMode();
                System::RenderSystem(scene, screen_rec);
            main_camera.EndMode();
            DrawFPS(10,10);
        window.EndDrawing();
    }
    return 0;
}