#ifndef ENTITY_H
#define ENTITY_H

#include "Component.h"
#include "TransformComponent.h"
#include <vector>
#include <memory>
#include <concepts>
#include <optional>
#include <iostream>

struct Entity{
    std::vector<std::unique_ptr<Component>> components;
    //Constructors
    Entity() {AddComponent<TransformComponent>();}
    //Copy constructor deleted
    Entity(const Entity&) = delete;
    //Move Constructor
    //Why move constructor?
    //Due to the components being unique_ptrs, a copy would transfer ownership of the unique_ptrs to the object that is "copying" which means the unique_ptrs of the original would
    //be deallocated due to the nature of unique_ptrs.
    //A move constructor will instead move the resources with the expectation that the item being moved from will no longer need those resources
    Entity(Entity&& other): components(std::move(other.components)){
        for(auto& c: components){
            c->object = this;
        }
    }
    //Add component
    template<std::derived_from<Component> T, typename... Ts>
    size_t AddComponent(Ts... args){
        auto comp = std::make_unique<T>(this, std::forward<Ts>(args)...);
        components.push_back(std::move(comp));
        return components.size() - 1;
    }
    //Get component of derived type
    template<std::derived_from<Component> T>
    std::optional<std::reference_wrapper<T>> GetComponent(){
        if constexpr (std::is_same_v<T, TransformComponent>){
            T* cast = dynamic_cast<T*>(components[0].get());
            if(cast) return *cast;
        }
        for(auto& c: components){
            T* cast = dynamic_cast<T*>(c.get());
            if(cast) return *cast;
        }
        return std::nullopt;
    }
    //Update 
    void update(float dt){
        for(auto& c : components){
            c->update(dt);
        }
    }
    void setup(){
        for(auto& c : components){
            c->setup();
        }
    }
};

#endif