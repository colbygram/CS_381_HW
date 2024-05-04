#include "System.hpp"
#include "Components.hpp"

void System::RenderSystem(SceneManager& scene, const raylib::Rectangle& screen){
    for(int e = 0; e < scene.entity_masks.size(); e++){
        if(!scene.HasComponent<Position>(e) || !scene.HasComponent<Tile>(e) || !scene.HasComponent<CollisionBox>(e)) continue;
        auto& crec = scene.GetComponent<CollisionBox>(e);
        if(!crec.m_rec.CheckCollision(screen)) continue;
        auto& tile = scene.GetComponent<Tile>(e);
        auto& pos = scene.GetComponent<Position>(e);
        DrawRectangle(pos.m_position.x, pos.m_position.y, 32, 32, tile.m_color);
    }
}

void System::MoveSystem(SceneManager& scene, Entity player, raylib::Camera2D& camera, raylib::BufferedInput& input, raylib::Rectangle& screen, float speed, float dt){
    if(!scene.HasComponent<Position>(player) && !scene.HasComponent<CollisionBox>(player)) return;
    auto& pos = scene.GetComponent<Position>(player);
    auto& crec = scene.GetComponent<CollisionBox>(player);

    pos.m_position.x += speed * dt * (-input["Left"].data.button.last_state + input["Right"].data.button.last_state);
    crec.m_rec.SetPosition(pos.m_position);

    //Cast pos from float to int. Stops stuttering due to DrawRectangle() drawing using int and not float
    camera.SetTarget(raylib::Vector2((int)pos.m_position.x, (int)pos.m_position.y));
    camera.SetOffset(raylib::Vector2(WIDTH/2,HEIGHT/1.25));

    screen.SetPosition(pos.m_position - raylib::Vector2(WIDTH/2,HEIGHT/1.25));
}
//Implement better: Create physics components for entities. make gravity global probably
void System::PhysicsSystem(SceneManager& scene, Entity player, bool& grounded, float gravity, float dt){
    if(!scene.HasComponent<Position>(player)) return;
    auto& pos = scene.GetComponent<Position>(player);
    float grav = gravity * dt;
    if(!grounded) pos.m_position.y += gravity;
}

void System::CollisionSystem(SceneManager& scene, Entity player, bool& grounded){
    if(!scene.HasComponent<CollisionBox>(player) || !scene.HasComponent<Tile>(player) || !scene.HasComponent<Position>(player)) return;
    auto& player_crec = scene.GetComponent<CollisionBox>(player);
    auto& player_tile = scene.GetComponent<Tile>(player);
    auto& player_pos = scene.GetComponent<Position>(player);
    for(size_t e = 0; e < scene.entity_masks.size(); e++){
        if(e == player) continue;
        if(!scene.HasComponent<CollisionBox>(e) || !scene.HasComponent<Tile>(e) || !scene.HasComponent<Position>(e)) continue;

        auto& crec = scene.GetComponent<CollisionBox>(e);
        auto& tile = scene.GetComponent<Tile>(e);
        auto& pos = scene.GetComponent<Position>(e);

        if(!CheckCollisionRecs(player_crec.m_rec, crec.m_rec)) continue;
        auto col_rec = GetCollisionRec(player_crec.m_rec, crec.m_rec);
        auto xy_col = player_pos.m_position - pos.m_position;

        if(col_rec.width < col_rec.height){
            if(xy_col.x > 0) player_pos.m_position.x += col_rec.width;
            else player_pos.m_position.x -= col_rec.width;
        }else{
            if(xy_col.y > 0) player_pos.m_position.y += col_rec.height;
            else{
                player_pos.m_position.y -= col_rec.height;
                grounded = true;
            } 
                
        }

        // switch(tile.current_tile){
        //     case ENEMY:
        //         break;
        //     case EXIT:
        //         break;
        //     default:
        //         //if x is positive, intersecting right side of object, else left
        //         //if y is positive, intersecting from the bottom of object, else top
        //         break;
        // }
    }
}