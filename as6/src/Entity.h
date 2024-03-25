#ifndef ENTITY_H
#define ENTITY_H

#include "Resources.h"

struct Entity{
    std::vector<std::unique_ptr<Component>> components;
    //Constructors
    Entity() {AddComponent<TransformComponent>();}
    Entity(const Entity&) = delete;
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
};

#endif