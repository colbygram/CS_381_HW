///@author: Colby Gramelspacher
///@ref: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
///      vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
///      forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles),
///      class examples (ComponentStorage implementation)

#include "Resources.hpp"

//Map 48x16 tiles
std::string map = "W.........................W....W...............W"
                  "W.........................W....W...............W"
                  "W...................FFFFFFW....W...............W"
                  "W.........FFFF.................W...............E"
                  "W...............................FFFFFFFFFFFFFFFW"
                  "W....................FFFFFFFF..................W"
                  "W.............OOO.................FFFFFFF......W"
                  "W........FFFFFFFF..............................W"
                  "W..........................................FFFFW"
                  "WFF.................FFFFFFF....................W"
                  "W......OO......................OOO.............W"
                  "WFFFFFFFFFFFFFF................FFFFFFFFFFFF....W"
                  "W.....................OO.......................W"
                  "W.S.................FFFFFFFFFFFFFFF............W"
                  "D..P...........................................W"
                  "FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF";

const static std::string texture_path = "assets/textures/";

void DrawMainMenu(const raylib::Rectangle& menu_rec, raylib::Texture2D& main_menu, raylib::Rectangle& start_button){
    DrawTextureRec(main_menu, menu_rec, raylib::Vector2::Zero(), WHITE);
    DrawRectangleRec(start_button, WHITE);
    DrawText("START", WIDTH/2-64 + 10, HEIGHT/2-32+2 + 100, 30, BLACK);
    DrawText("ROBOT RUN", WIDTH/2 - MeasureText("ROBOT RUN", 100)/2, HEIGHT/4, 100 , BLACK);
}

void SetupScene(SceneManager& scene, const std::string& map, Entity& player, Entity& spawn, std::vector<raylib::Texture>& textures){
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
                tile.texture = &textures[2];
                break;
            case 'F':
                tile.current_tile = FLOOR;
                tile.texture = &textures[3];
                crec.m_rec.height = 8;
                break;
            case 'O':
                tile.current_tile = ENEMY;
                tile.texture = &textures[1];
                break;
            case 'D':
                tile.current_tile = DOOR;
                tile.texture = &textures[4];
                break;
            case 'P':
                tile.current_tile = PLAYER;
                tile.texture = &textures[0];
                player = e;
                break;
            case 'E':
                tile.current_tile = EXIT;
                tile.texture = &textures[5];
                break;
            case 'S':
                tile.current_tile = SPAWN;
                tile.texture = nullptr;
                spawn = e;
                break;
            default:
                tile.current_tile = NONE;
                break;
        }
        row_check--;
    }
}
//////////////////////////////////////////// MAIN //////////////////////////////////////////////////////////////
int main(){
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 9");
    raylib::Camera2D main_camera(raylib::Vector2::Zero(), raylib::Vector2::Zero(), 0, 1);

    raylib::BufferedInput player_inputs;
    player_inputs["Left"] = raylib::Action::key(KEY_A).move();
    player_inputs["Right"] = raylib::Action::key(KEY_D).move();
    player_inputs["Jump"] = raylib::Action::key(KEY_SPACE).move();

    std::vector<raylib::Texture2D> textures;
    textures.emplace_back(raylib::Texture2D{texture_path + "Player.png"});
    textures.emplace_back(raylib::Texture2D{texture_path + "Enemy.png"});
    textures.emplace_back(raylib::Texture2D{texture_path + "Wallt.png"});
    textures.emplace_back(raylib::Texture2D{texture_path + "Plank.png"});
    textures.emplace_back(raylib::Texture2D{texture_path + "Entrance.png"});
    textures.emplace_back(raylib::Texture2D{texture_path + "Exit.png"});
    SceneManager scene;
    Entity player, spawn;
    SetupScene(scene, map, player, spawn, textures);

    auto& player_phys = scene.AddComponent<Physics>(player);
    player_phys.velocity = raylib::Vector2::Zero();
    player_phys.jump_speed = 1;
    player_phys.max_speed = 1;
    
    auto& player_state = scene.AddComponent<State>(player);
    player_state.is_grounded = true;
    player_state.is_jumping = false;
    player_state.move_direction = 0;

    auto& player_pos = scene.GetComponent<Position>(player);
    auto& player_tile = scene.GetComponent<Tile>(player);
    auto& player_cbox = scene.GetComponent<CollisionBox>(player);

    raylib::Rectangle screen_rec(0,0, WIDTH, HEIGHT);
    raylib::Texture2D main_menu(texture_path + "main_menu.jpg");

    float jump_timer = JUMP_TIME;
    bool in_menu = true;
    raylib::Rectangle start_button(WIDTH/2-64,HEIGHT/2-32 +100,128,32);

    while (!window.ShouldClose()) {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(CheckCollisionPointRec(GetMousePosition(), start_button)) in_menu = false;
        }
        if(!in_menu){
            player_inputs.PollEvents();
            System::MoveSystem(player_inputs, player_state, player_pos);
            System::PhysicsSystem(player_phys, player_pos, player_cbox, player_state, jump_timer, GetFrameTime());
            System::CollisionSystem(scene, player, spawn, player_pos, player_cbox, player_state, player_phys);
            System::CameraSystem(player_pos, main_camera, screen_rec);
        }
        window.BeginDrawing();
            window.ClearBackground(GRAY);
            main_camera.BeginMode();
                if(in_menu) DrawMainMenu(screen_rec, main_menu, start_button);
                else System::RenderSystem(scene, screen_rec);
            main_camera.EndMode();
            DrawFPS(10,10);
        window.EndDrawing();
    }
    return 0;
}