#ifndef ENTITYCOMPONENT_H
#define ENTITYCOMPONENT_H

#include <optional>
#include <vector>
#include <memory>
#include <concepts>
#include "../raylib-cpp/include/raylib-cpp.hpp"

struct Entity{
    std::vector<std::unique_ptr<struct Component>> components;

    template<std::derived_from<struct Component> T, typename... Ts>
    size_t AddComponent(Ts... args){
        auto comp = std::make_unique<T>(*this, std::forward<Ts>(args)...);
        components.push_back(std::move(comp));
        return components.size() - 1;
    }

    template<std::derived_from<struct Component> T>
    std::optional<std::reference_wrapper<T>> GetComponent(){
        if constexpr (std::is_same_v<T, struct TransformComponent>){
            T* cast = dynamic_cast<T>(components[0].get());
            if(cast) return *cast;
        }
        for(auto& c: components){
            T* cast = dynamic_cast<T>(c.get());
            if(cast) return *cast;
        }
        return std::nullopt;
    }

    Entity(){AddComponent<struct TransformComponent>();}
    Entity(const Entity&) = delete;
    Entity(Entity&& other): components(std::move(other.components)){
        for(auto& c: components){
            c->entity = this;
        }
    }
};

struct Component{
    Entity* entity;
    Component(Entity& en) : entity(&en){}
    virtual void setup() = 0;
    virtual void cleanup() = 0;
    virtual void update(float dt) = 0;

};

struct TransformComponent : public Component{
    using Component::Component;
    raylib::Vector3 position = {0,0,0};
    raylib::Quaternion rotation = raylib::Quaternion::Identity();
};

struct RenderComponent : public Component{
    raylib::Model model;
    void update(float dt) override{
        auto ref = entity->GetComponent<TransformComponent>();
        if(!ref) return;
        auto& transform = ref->get();
        auto [axis, angle] = transform.rotation.ToAxisAngle();

        model.Draw(transform.position, axis, angle);
    }
};

#endif