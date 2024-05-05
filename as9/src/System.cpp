#include "System.hpp"
#include "Components.hpp"

                   
void System::RenderSystem(SceneManager& scene, const raylib::Rectangle& screen){
    for(int e = 0; e < scene.entity_masks.size(); e++){
        if(!scene.HasComponent<Position>(e) || !scene.HasComponent<Tile>(e) || !scene.HasComponent<CollisionBox>(e)) continue;
        auto& crec = scene.GetComponent<CollisionBox>(e);
        if(!crec.m_rec.CheckCollision(screen)) continue;
        auto& tile = scene.GetComponent<Tile>(e);
        auto& pos = scene.GetComponent<Position>(e);
        if(!tile.texture) continue;
        DrawTextureRec(*tile.texture, crec.m_rec, pos.m_position, WHITE);
    }
}

void System::MoveSystem(raylib::BufferedInput& input, State& player_state, Position& player_pos){
    player_state.move_direction = -input["Left"].data.button.last_state + input["Right"].data.button.last_state;
    if(input["Jump"].data.button.last_state && player_state.is_grounded){
        player_state.is_jumping = true;
        player_state.is_grounded = false;
    }

}

void System::PhysicsSystem(Physics& phys, Position& pos, CollisionBox& cbox, State& state, float& jump_timer, float dt){
    //Handle movement Physics
    if(state.move_direction == 0) phys.velocity.x = 0;
    phys.velocity.x += state.move_direction * 20 * dt;
    if(phys.velocity.x < -phys.max_speed) phys.velocity.x = -phys.max_speed;
    else if(phys.velocity.x > phys.max_speed) phys.velocity.x = phys.max_speed;

    //Handle jumping physics
    if(state.is_jumping && jump_timer > 0){
        phys.velocity.y -= phys.jump_speed * 20 * dt;
        jump_timer -= dt;
    } else if(jump_timer <= 0){
        jump_timer = JUMP_TIME;
        state.is_jumping = false;
    }

    //Handle gravity
    if(!state.is_grounded && !state.is_jumping) {
        phys.velocity.y += GRAVITY * dt;
    }
    //Update player position and collision box position
    pos.m_position += phys.velocity;
    cbox.m_rec.SetPosition(pos.m_position);
}

void System::CollisionSystem(SceneManager& scene, Entity player, Entity spawn, Position& player_pos, CollisionBox& player_cbox, State& player_state, Physics& player_phys){
    //Used to determine if object is grounded
    bool did_collide = false;
    auto& spawn_pos = scene.GetComponent<Position>(spawn);
    for(size_t e = 0; e < scene.entity_masks.size(); e++){
        if(e == player) continue;
        if(!scene.HasComponent<CollisionBox>(e) || !scene.HasComponent<Tile>(e) || !scene.HasComponent<Position>(e)) continue;
        auto& crec = scene.GetComponent<CollisionBox>(e);
        auto& tile = scene.GetComponent<Tile>(e);
        auto& pos = scene.GetComponent<Position>(e);

        if(!CheckCollisionRecs(player_cbox.m_rec, crec.m_rec)) continue;
        did_collide = true;
        auto col_rec = GetCollisionRec(player_cbox.m_rec, crec.m_rec);
        auto pos_diff = player_pos.m_position - pos.m_position;

        if(col_rec.width < col_rec.height){ //X based collision
            if(pos_diff.x > 0) player_pos.m_position.x += col_rec.width; //Collided on right side
            else player_pos.m_position.x -= col_rec.width;// Collided on left
            player_phys.velocity.x = 0;
        }
        else{ //Y based collision
            if(pos_diff.y > 0) {// if y is positive, collided at bottom
                player_pos.m_position.y += col_rec.height;
                player_state.is_jumping = false;
            }
            else{ // else collided on top
                player_pos.m_position.y -= col_rec.height;
                player_state.is_grounded = true;  
            }   
            player_phys.velocity.y = 0;
        }
        if(tile.current_tile == ENEMY){
            player_pos.m_position = spawn_pos.m_position;
        }
    }
    if(!did_collide){
        player_state.is_grounded = false;
    }
}
void System::CameraSystem(struct Position& player_pos, raylib::Camera2D& camera, raylib::Rectangle& screen){
    camera.SetTarget(raylib::Vector2((int)player_pos.m_position.x, (int)player_pos.m_position.y));
    camera.SetOffset(raylib::Vector2(WIDTH/2, HEIGHT/1.5));
    screen.SetPosition(player_pos.m_position - raylib::Vector2(WIDTH/2, HEIGHT/1.5));
}