//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//           vector magnitude (https://socratic.org/questions/how-do-you-find-the-vector-v-with-the-given-magnitude-of-9-and-in-the-same-direc), 
//           forward vector calculation (https://gamedev.stackexchange.com/questions/190054/how-to-calculate-the-forward-up-right-vectors-using-the-rotation-angles)
//Problem: 
//Solution: 
//Extra Credit: 

#include "Resources.h"

extern size_t globalComponentCounter;

template<typename T>
size_t GetComponentID(T reference = {}) {
    static size_t id = globalComponentCounter++;
    return id;
}

using Entity = uint8_t; 
////////////////////////////////////////COMPONENT STORAGE//////////////////////////////////////////////
struct ComponentStorage {
    size_t elementSize = -1;
    std::vector<std::byte> data;

    ComponentStorage() : elementSize(-1), data(1, std::byte{0}) {}
    ComponentStorage(size_t elementSize) : elementSize(elementSize) { data.reserve(5 * elementSize); }
    
    template<typename Tcomponent>
    ComponentStorage(Tcomponent reference = {}) : ComponentStorage(sizeof(Tcomponent)) {}

    template<typename Tcomponent>
    Tcomponent& Get(Entity e) {
        assert(sizeof(Tcomponent) == elementSize);
        assert(e < (data.size() / elementSize));
        return *(Tcomponent*)(data.data() + e * elementSize);
    }

    template<typename Tcomponent>
    std::pair<Tcomponent&, size_t> Allocate(size_t count = 1) {
        assert(sizeof(Tcomponent) == elementSize);
        assert(count < 100);
        auto originalEnd = data.size();
        data.insert(data.end(), elementSize * count, std::byte{0});
        for(size_t i = 0; i < count - 1; i++) // Skip the last one
            new(data.data() + originalEnd + i * elementSize) Tcomponent();
        return {
            *new(data.data() + data.size() - elementSize) Tcomponent(),
            data.size() / elementSize
        };
    }

    template<typename Tcomponent>
    Tcomponent& GetOrAllocate(Entity e) {
        assert(sizeof(Tcomponent) == elementSize);
        size_t size = data.size() / elementSize;
        if (size <= e)
            Allocate<Tcomponent>(std::max<int64_t>(int64_t(e) - size + 1, 1));
        return Get<Tcomponent>(e);
    }
};
//////////////////////////////SCENE///////////////////////////////////////
template<typename Storage = ComponentStorage>
struct Scene {
    std::vector<std::vector<bool>> entityMasks;
    std::vector<Storage> storages = {Storage()};

    template<typename Tcomponent>
    Storage& GetStorage() {
        size_t id = GetComponentID<Tcomponent>();
        if(storages.size() <= id)
            storages.insert(storages.cend(), std::max<int64_t>(id - storages.size(), 1), Storage());
        if (storages[id].elementSize == std::numeric_limits<size_t>::max())
            storages[id] = Storage(Tcomponent{});
        return storages[id];
    }

    Entity CreateEntity() {
        Entity e = entityMasks.size();
        entityMasks.emplace_back(std::vector<bool>{false});
        return e;
    }

    template<typename Tcomponent>
    Tcomponent& AddComponent(Entity e) {
        size_t id = GetComponentID<Tcomponent>();
        auto& eMask = entityMasks[e];
        if(eMask.size() <= id)
            eMask.resize(id + 1, false);
        eMask[id] = true;
        return GetStorage<Tcomponent>().template GetOrAllocate<Tcomponent>(e);
    }

    template<typename Tcomponent>
    void RemoveComponent(Entity e) {
        size_t id = GetComponentID<Tcomponent>();
        auto& eMask = entityMasks[e];
        if(eMask.size() > id)
            eMask[id] = false;
    }

    template<typename Tcomponent>
    Tcomponent& GetComponent(Entity e) {
        size_t id = GetComponentID<Tcomponent>();
        assert(entityMasks[e][id]);
        return GetStorage<Tcomponent>().template Get<Tcomponent>(e);
    }

    template<typename Tcomponent>
    bool HasComponent(Entity e) {
        size_t id = GetComponentID<Tcomponent>();
        return entityMasks[e].size() > id && entityMasks[e][id];
    }
};

///////////////////////////////////////////COMPONENTS/////////////////////////////////////////////////
struct Physics2D{
    raylib::Vector3 velocity;
    float max_speed;
    float speed;
    float acceleration;
    float turn_rate;
    int turn_yaw;
};

struct Physics3D{
    raylib::Vector3 velocity;
    float max_speed;
    float speed;
    float acceleration;
    float turn_rate;
    int turn_yaw, turn_pitch;
};
struct TransformC{
    raylib::Vector3 position;
    raylib::Quaternion rotation;
    raylib::Vector3 euler_angles;
    raylib::Vector3 forward_vector;
};

struct Render{
    raylib::Model* model;
    bool selected;
};

struct Inputs{
    bool is_plane;
};

////////////////////////////////SYSTEMS///////////////////////////////////////////
void RenderingSystem(Scene<ComponentStorage>& scene){
    for(Entity e = 0; e < scene.entityMasks.size(); e++){
        if(!scene.HasComponent<Render>(e)) continue;
        if(!scene.HasComponent<TransformC>(e)) continue;
        //Grab transform component if it exists, else return
        auto& render = scene.GetComponent<Render>(e);
        auto& transform = scene.GetComponent<TransformC>(e);
        //Store temporary version of model's transform
        auto temp = render.model->transform;
        //Convert quaternion to axis angle
        auto [axis, angle] = transform.rotation.ToAxisAngle();
        //Set models new transform
        render.model->SetTransform(raylib::Transform(render.model->transform).Translate(transform.position));
        render.model->Draw({});
        if(render.selected) render.model->GetTransformedBoundingBox().Draw();
        //Reset old model transform
        render.model->transform = temp;
    }
}
void CameraSystem(Scene<ComponentStorage>& scene, Entity e, raylib::Camera3D& camera, raylib::Vector3 offset){
    if(!scene.HasComponent<TransformC>(e)) return;
    auto& trans = scene.GetComponent<TransformC>(e);
    camera.SetPosition(trans.position + offset); 
    camera.SetTarget(trans.position);
}
void BoatSystem(Scene<ComponentStorage>& scene, raylib::BufferedInput& inputs, int current_index, bool& inputs_pressed, float dt){
    if(!scene.HasComponent<Render>(current_index) || !scene.HasComponent<TransformC>(current_index) || !scene.HasComponent<Physics2D>(current_index) || !scene.HasComponent<Inputs>(current_index)) return;
    auto& inp = scene.GetComponent<Inputs>(current_index);
    if(inp.is_plane) return;
    auto& render = scene.GetComponent<Render>(current_index);
    if(!render.selected) return;
    auto& trans = scene.GetComponent<TransformC>(current_index);
    auto& phys = scene.GetComponent<Physics2D>(current_index);
    if(!inputs_pressed && inputs["Forward"].data.button.last_state) {
        phys.speed += phys.acceleration * dt;
        inputs_pressed = true;
    }
    if(!inputs_pressed && inputs["Backward"].data.button.last_state) {
        phys.speed -= phys.acceleration * dt;
        inputs_pressed = true;
    }
    if(!inputs_pressed && inputs["Left"].data.button.last_state) {
        phys.turn_yaw--;
        if(phys.turn_yaw<-1) phys.turn_yaw = 0;
        inputs_pressed = true;
    }
    if(!inputs_pressed && inputs["Right"].data.button.last_state) {
        phys.turn_yaw++;
        if(phys.turn_yaw>1) phys.turn_yaw = 0;
        inputs_pressed = true;
    }
    if(!inputs["Forward"].data.button.last_state &&
       !inputs["Backward"].data.button.last_state &&
       !inputs["Left"].data.button.last_state &&
       !inputs["Right"].data.button.last_state){
        inputs_pressed = false;
    }
}
void PlaneSystem(Scene<ComponentStorage>& scene, raylib::BufferedInput& inputs, int current_index, bool& inputs_pressed, float dt){
    if(!scene.HasComponent<Render>(current_index) || !scene.HasComponent<TransformC>(current_index) || !scene.HasComponent<Physics3D>(current_index) || !scene.HasComponent<Inputs>(current_index)) return;
    auto& inp = scene.GetComponent<Inputs>(current_index);
    if(!inp.is_plane) return;
    auto& render = scene.GetComponent<Render>(current_index);
    if(!render.selected) return;
    auto& trans = scene.GetComponent<TransformC>(current_index);
    auto& phys = scene.GetComponent<Physics3D>(current_index);
    if(!inputs_pressed && inputs["Forward"].data.button.last_state) {
        phys.speed += phys.acceleration * dt;
        inputs_pressed = true;
    }
    if(!inputs_pressed && inputs["Backward"].data.button.last_state) {
        phys.speed -= phys.acceleration * dt;
        inputs_pressed = true;
    }
    if(!inputs_pressed && inputs["Left"].data.button.last_state) {
        phys.turn_yaw--;
        if(phys.turn_yaw<-1) phys.turn_yaw = 0;
        inputs_pressed = true;
    }
    if(!inputs_pressed && inputs["Right"].data.button.last_state) {
        phys.turn_yaw++;
        if(phys.turn_yaw>1) phys.turn_yaw = 0;
        inputs_pressed = true;
    }
    if(!inputs_pressed && inputs["Up"].data.button.last_state) {
        phys.turn_pitch++;
        if(phys.turn_pitch>1) phys.turn_pitch = 0;
        inputs_pressed = true;
    }
    if(!inputs_pressed && inputs["Down"].data.button.last_state) {
        phys.turn_pitch--;
        if(phys.turn_pitch<-1) phys.turn_pitch = 0;
        inputs_pressed = true;
    }
    if(!inputs["Forward"].data.button.last_state &&
       !inputs["Backward"].data.button.last_state &&
       !inputs["Left"].data.button.last_state &&
       !inputs["Right"].data.button.last_state &&
       !inputs["Up"].data.button.last_state &&
       !inputs["Down"].data.button.last_state){
        inputs_pressed = false;
    }
}
raylib::Vector3 CalculateForwardVector(raylib::Vector3 eulers){
    raylib::Vector3 forward_vector;
    forward_vector.x = cos(eulers.x) * sin(eulers.y);
    forward_vector.y = -sin(eulers.x);
    forward_vector.z = cos(eulers.x) * cos(eulers.y);
    return forward_vector;
}
void Physics2DSystem(Scene<ComponentStorage>& scene, float dt){
    for(Entity e; e < scene.entityMasks.size(); e++){
        if(!scene.HasComponent<Physics2D>(e) || !scene.HasComponent<TransformC>(e)) continue;
        auto& phys = scene.GetComponent<Physics2D>(e);
        auto& trans = scene.GetComponent<TransformC>(e);
        trans.euler_angles.y += (phys.turn_yaw * phys.turn_rate * dt);
        trans.rotation = raylib::Quaternion::FromEuler(trans.euler_angles);
        trans.forward_vector = CalculateForwardVector(trans.euler_angles);
        phys.velocity = (trans.forward_vector.Normalize() * phys.speed);
        trans.position += phys.velocity;
    }
}
void Physics3DSystem(Scene<ComponentStorage>& scene, float dt){
    for(Entity e; e < scene.entityMasks.size(); e++){
        if(!scene.HasComponent<Physics3D>(e) || !scene.HasComponent<TransformC>(e)) continue;
        auto& phys = scene.GetComponent<Physics3D>(e);
        auto& trans = scene.GetComponent<TransformC>(e);
        trans.euler_angles.y += (phys.turn_yaw * phys.turn_rate * dt);
        trans.euler_angles.x += (phys.turn_pitch * phys.turn_rate * dt);
        trans.rotation = raylib::Quaternion::FromEuler(trans.euler_angles);
        trans.forward_vector = CalculateForwardVector(trans.euler_angles);
        phys.velocity = (trans.forward_vector.Normalize() * phys.speed);
        trans.position += phys.velocity;
    }
}

///////////////////////////Utility////////////////////////
void VehicleControls(const bool toggle);
/////////////////////////MAIN///////////////////////////////////////////
int main(){
    //Consts
    const int WIDTH = 1920/2;
    const int HEIGHT = 1080/2;
    const int max_size = 10;
    const std::string mesh_path = "assets/meshes/";
    const std::string texture_path = "assets/textures/";
    const std::string plane_file = "PolyPlane.glb";
    const std::string boat_files[5] = {"SmitHouston_Tug.glb", "CargoG_HOSBrigadoon.glb", "Container_ShipLarge.glb", "OilTanker.glb", "OrientExplorer.glb"};
    const std::string water_file = "water.jpg";
    
    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 8");
    raylib::Camera3D main_camera(raylib::Vector3(250,100,0), raylib::Vector3(0,0,0), raylib::Vector3(0,1,0), 90, CAMERA_PERSPECTIVE);
    raylib::Vector3 camera_offset = {0,50,-100};
    
    //water plane and texture load
    Mesh plane_mesh = raylib::Mesh::Plane(10'000, 10'000, 16, 16, 10);
    raylib::Model water_plane = ((raylib::Mesh*)&plane_mesh)->LoadModelFrom();
    raylib::Texture2D water(texture_path + water_file);
    water.SetFilter(TEXTURE_FILTER_BILINEAR);
    water.SetWrap(TEXTURE_WRAP_REPEAT);
    water_plane.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = water;

    water_plane.GetTransformedBoundingBox();
    
    //Boat and Plane model load
    raylib::Model plane_model(mesh_path+plane_file);
    //Plane models default orientation needed to be updated by -90 degrees
    plane_model.SetTransform(raylib::Transform(plane_model.transform).RotateY(raylib::Radian(-1.571)));
    raylib::Model boat01_model(mesh_path+boat_files[0]), boat02_model(mesh_path+boat_files[1]), boat03_model(mesh_path+boat_files[2]), boat04_model(mesh_path+boat_files[3]), boat05_model(mesh_path+boat_files[4]);
    //each boat needs to be adjusted
    boat02_model.SetTransform(raylib::Transform(boat02_model.transform).RotateX(raylib::Radian(1.571)).RotateY(PI).Scale(0.01,0.01,0.01));
    boat03_model.SetTransform(raylib::Transform(boat03_model.transform).RotateX(raylib::Radian(1.571)).Scale(0.01,0.01,0.01));
    boat04_model.SetTransform(raylib::Transform(boat04_model.transform).RotateX(raylib::Radian(1.571)).Scale(0.01,0.01,0.01));
    boat05_model.SetTransform(raylib::Transform(boat05_model.transform).RotateX(raylib::Radian(1.571)).RotateY(PI).Scale(0.01,0.01,0.01));
    
    //Inputs
    raylib::BufferedInput inputs;

    //Scene init
    Scene<ComponentStorage> scene;

    //Plane entities
    Entity e0 = scene.CreateEntity(), e1 = scene.CreateEntity(), e2 = scene.CreateEntity(), e3 = scene.CreateEntity(), e4 = scene.CreateEntity();

    auto& render0 = scene.AddComponent<Render>(e0); 
    auto& transform0 = scene.AddComponent<TransformC>(e0);
    auto& inp0 = scene.AddComponent<Inputs>(e0);
    inp0.is_plane = true;
    auto& phys0 = scene.AddComponent<Physics3D>(e0);
    transform0.position = raylib::Vector3(0,50,0);
    render0.model = &plane_model;
    transform0.rotation = raylib::Quaternion::Identity();
    phys0.acceleration = 20;
    phys0.max_speed = 5;
    phys0.speed = 0;
    phys0.turn_pitch = 0;
    phys0.turn_yaw = 0;
    phys0.turn_rate = 0.25;
    phys0.velocity = raylib::Vector3::Zero();

    auto& render1 = scene.AddComponent<Render>(e1); 
    auto& transform1 = scene.AddComponent<TransformC>(e1); 
    auto& inp1 = scene.AddComponent<Inputs>(e1);
    inp1.is_plane = true;
    auto& phys1 = scene.AddComponent<Physics3D>(e1);
    transform1.position = raylib::Vector3(0,50,-75);
    render1.model = &plane_model;
    transform1.rotation = raylib::Quaternion::Identity();
    phys1.acceleration = 20;
    phys1.max_speed = 5;
    phys1.speed = 0;
    phys1.turn_pitch = 0;
    phys1.turn_yaw = 0;
    phys1.turn_rate = 0.25;
    phys1.velocity = raylib::Vector3::Zero();

    auto& render2 = scene.AddComponent<Render>(e2); 
    auto& transform2 = scene.AddComponent<TransformC>(e2);  
    auto& inp2 = scene.AddComponent<Inputs>(e2);
    inp2.is_plane = true;
    auto& phys2 = scene.AddComponent<Physics3D>(e2);
    transform2.position = raylib::Vector3(0,50,75);
    render2.model = &plane_model;
    transform2.rotation = raylib::Quaternion::Identity();
    phys2.acceleration = 20;
    phys2.max_speed = 5;
    phys2.speed = 0;
    phys2.turn_pitch = 0;
    phys2.turn_yaw = 0;
    phys2.turn_rate = 0.25;
    phys2.velocity = raylib::Vector3::Zero();

    auto& render3 = scene.AddComponent<Render>(e3); 
    auto& transform3 = scene.AddComponent<TransformC>(e3);  
    auto& inp3 = scene.AddComponent<Inputs>(e3);
    inp3.is_plane = true;
    auto& phys3 = scene.AddComponent<Physics3D>(e3);
    transform3.position = raylib::Vector3(0,50,-150);
    render3.model = &plane_model;
    transform3.rotation = raylib::Quaternion::Identity();
    phys3.acceleration = 20;
    phys3.max_speed = 5;
    phys3.speed = 0;
    phys3.turn_pitch = 0;
    phys3.turn_yaw = 0;
    phys3.turn_rate = 0.25;
    phys3.velocity = raylib::Vector3::Zero();

    auto& render4 = scene.AddComponent<Render>(e4); 
    auto& transform4 = scene.AddComponent<TransformC>(e4); 
    auto& inp4 = scene.AddComponent<Inputs>(e4);
    inp4.is_plane = true; 
    auto& phys4 = scene.AddComponent<Physics3D>(e4);
    transform4.position = raylib::Vector3(0,50,150);
    render4.model = &plane_model;
    transform4.rotation = raylib::Quaternion::Identity();
    phys4.acceleration = 20;
    phys4.max_speed = 5;
    phys4.speed = 0;
    phys4.turn_pitch = 0;
    phys4.turn_yaw = 0;
    phys4.turn_rate = 0.25;
    phys4.velocity = raylib::Vector3::Zero();

    //Boat entities
    Entity e5 = scene.CreateEntity(), e6 = scene.CreateEntity(), e7 = scene.CreateEntity(), e8 = scene.CreateEntity(), e9 = scene.CreateEntity();

    auto& render5 = scene.AddComponent<Render>(e5); 
    auto& transform5 = scene.AddComponent<TransformC>(e5); 
    auto& inp5 = scene.AddComponent<Inputs>(e5);
    inp5.is_plane = false; 
    auto& phys5 = scene.AddComponent<Physics2D>(e5);
    transform5.position = raylib::Vector3(0,0,0);
    render5.model = &boat01_model;
    transform5.rotation = raylib::Quaternion::Identity();
    phys5.acceleration = 20;
    phys5.max_speed = 5;
    phys5.speed = 0;
    phys5.turn_yaw = 0;
    phys5.turn_rate = 0.25;
    phys5.velocity = raylib::Vector3::Zero();

    auto& render6 = scene.AddComponent<Render>(e6); 
    auto& transform6 = scene.AddComponent<TransformC>(e6);  
    auto& inp6 = scene.AddComponent<Inputs>(e6);
    inp6.is_plane = false; 
    auto& phys6 = scene.AddComponent<Physics2D>(e6);
    transform6.position = raylib::Vector3(0,0,-75);
    render6.model = &boat02_model;
    transform6.rotation = raylib::Quaternion::Identity();
    phys6.acceleration = 10;
    phys6.max_speed = 3;
    phys6.speed = 0;
    phys6.turn_yaw = 0;
    phys6.turn_rate = 0.25;
    phys6.velocity = raylib::Vector3::Zero();

    auto& render7 = scene.AddComponent<Render>(e7); 
    auto& transform7 = scene.AddComponent<TransformC>(e7);  
    auto& inp7 = scene.AddComponent<Inputs>(e7);
    inp7.is_plane = false; 
    auto& phys7 = scene.AddComponent<Physics2D>(e7);
    transform7.position = raylib::Vector3(0,0,75);
    render7.model = &boat03_model;
    transform7.rotation = raylib::Quaternion::Identity();
    phys7.acceleration = 10;
    phys7.max_speed = 4;
    phys7.speed = 0;
    phys7.turn_yaw = 0;
    phys7.turn_rate = 0.25;
    phys7.velocity = raylib::Vector3::Zero();

    auto& render8 = scene.AddComponent<Render>(e8); 
    auto& transform8 = scene.AddComponent<TransformC>(e8);
    auto& inp8 = scene.AddComponent<Inputs>(e8);
    inp8.is_plane = false;   
    auto& phys8 = scene.AddComponent<Physics2D>(e8);
    transform8.position = raylib::Vector3(0,0,-150);
    render8.model = &boat04_model;
    transform8.rotation = raylib::Quaternion::Identity();
    phys8.acceleration = 15;
    phys8.max_speed = 5;
    phys8.speed = 0;
    phys8.turn_yaw = 0;
    phys8.turn_rate = 0.25;
    phys8.velocity = raylib::Vector3::Zero();

    auto& render9 = scene.AddComponent<Render>(e9); 
    auto& transform9 = scene.AddComponent<TransformC>(e9); 
    auto& inp9 = scene.AddComponent<Inputs>(e9);
    inp9.is_plane = false;  
    auto& phys9 = scene.AddComponent<Physics2D>(e9);
    transform9.position = raylib::Vector3(0,0,150);
    render9.model = &boat05_model;
    transform9.rotation = raylib::Quaternion::Identity();
    phys9.acceleration = 20;
    phys9.max_speed = 5;
    phys9.speed = 0;
    phys9.turn_yaw = 0;
    phys9.turn_rate = 0.25;
    phys9.velocity = raylib::Vector3::Zero();

    Entity entities[10] = {e0,e1,e2,e3,e4,e5,e6,e7,e8,e9};
    int current_index = 0;

    inputs["Forward"] = raylib::Action::key(KEY_W).move();
    inputs["Backward"] = raylib::Action::key(KEY_S).move();
    inputs["Left"] = raylib::Action::key(KEY_A).move();
    inputs["Right"] = raylib::Action::key(KEY_D).move();
    inputs["Up"] = raylib::Action::key(KEY_UP).move();
    inputs["Down"] = raylib::Action::key(KEY_DOWN).move();
    inputs["Reset"] = raylib::Action::key(KEY_SPACE).move();

    //Control menu toggle
    bool control_toggle = false;
    bool inputs_pressed = false;

    if(scene.HasComponent<Render>(entities[current_index])) scene.GetComponent<Render>(entities[current_index]).selected = true;
    //Game loop
    while (!window.ShouldClose()) {
        inputs.PollEvents();
        if(IsKeyPressed(KEY_C)) control_toggle = !control_toggle;
        if(IsKeyPressed(KEY_TAB) && scene.HasComponent<Render>(entities[current_index])){
            scene.GetComponent<Render>(entities[current_index]).selected = false;
            current_index = (current_index + 1) % 10;
            scene.GetComponent<Render>(entities[current_index]).selected = true;
        }
        BoatSystem(scene, inputs, entities[current_index], inputs_pressed, GetFrameTime());
        PlaneSystem(scene, inputs, entities[current_index], inputs_pressed, GetFrameTime());
        Physics2DSystem(scene, GetFrameTime());
        Physics3DSystem(scene, GetFrameTime());
        //Render
        window.BeginDrawing();
            window.ClearBackground(GRAY);
            CameraSystem(scene, entities[current_index], main_camera, camera_offset);
            main_camera.BeginMode();
                water_plane.Draw({});
                RenderingSystem(scene);
            main_camera.EndMode();
            VehicleControls(control_toggle);
        window.EndDrawing();
    }
    return 0;
}

void VehicleControls(const bool toggle){
    if(toggle){
        DrawRectangle(0,0,GetRenderWidth(), GetRenderHeight(), raylib::Color(0,0,0,150));
        DrawText("PLANE", GetRenderWidth() - MeasureText("PLANE",20) - 5, 10, 20, RED);
        DrawText("Use [W] and [S] to accelerate and decelerate", GetRenderWidth() - MeasureText("Use [W] and [S] to accelerate and decelerate",20) - 5, 30, 20, GREEN);
        DrawText("Use ARROW KEYS to turn the plane and ascend/descend", GetRenderWidth() - MeasureText("Use ARROW KEYS to turn the plane and ascend/descend",20) - 5, 50, 20, GREEN);
        DrawText("Use TAB to switch between vehicles", GetRenderWidth() - MeasureText("Use TAB to switch between vehicles",20) - 5, 70, 20, GREEN);
        DrawText("BOAT", GetRenderWidth() - MeasureText("BOAT",20) - 5, 100, 20, RED);
        DrawText("Use [W] and [S] to accelerate and decelerate", GetRenderWidth() - MeasureText("Use [W] and [S] to accelerate and decelerate",20) - 5, 120, 20, GREEN);
        DrawText("Use [A] and [D] to turn the boats left and right", GetRenderWidth() - MeasureText("Use [A] and [D] to turn the boats left and right",20) - 5, 140, 20, GREEN);
        DrawText("Use TAB to switch between vehicles", GetRenderWidth() - MeasureText("Use TAB to switch between vehicles",20) - 5, 160, 20, GREEN);
    }else{
        DrawText("[C] for controls", GetRenderWidth() - MeasureText("[C] for controls",20) - 5, 10, 20, GREEN);
        DrawText("[TAB] to switch between entities", GetRenderWidth() - MeasureText("[TAB] to switch between entities",20) - 5, 30, 20, GREEN);
    }
}