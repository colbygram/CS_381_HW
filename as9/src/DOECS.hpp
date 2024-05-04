#ifndef DOECS_HPP
#define DOECS_HPP

#include <memory>
#include <concepts>
#include <vector>
#include <deque>
#include <iostream>
#include <ranges>
#include <bitset>
#include <span>
#include <variant>
#include <cassert>
#include "Generator.hpp"
#include "Globals.hpp"

/// @brief 
/// Type erasured storage method for component types. Necessary when components are unknown in advance and the data structures must dynamically adapt
/// @var 
/// component_data: a vector of bytes that will be allocated for dynamic
/// @ref
/// Basically exact implementation from class. Few changes here and there. Don't know of any other effective type erasure methods beyond this one

class ComponentStorage{
public:
    ComponentStorage() : element_size(-1), component_data(1, std::byte{0}){}
    template<typename Tcomp>
    //std::vector<>::reserve sets the capacity of the vector, but does not change the size of it
    //Use max amount of entities and sizeof(Tcomp) to determine the max bytes to be used
    ComponentStorage(Tcomp ref = {}) : element_size(sizeof(Tcomp)) {component_data.reserve(MAX_ENTITIES * sizeof(Tcomp));}

    template<typename Tcomp>
    Tcomp& Get(Entity e_index){
        assert(element_size == sizeof(Tcomp));
        return *(Tcomp*)(component_data.data() + (e_index * element_size));
    }
    template<typename Tcomp>
    void Allocate(size_t amount){
        assert(sizeof(Tcomp) == element_size);
        assert((component_data.size() / element_size) + amount <= MAX_ENTITIES);
        size_t prior_end = component_data.size();
        component_data.insert(component_data.end(), amount * element_size, std::byte{0});
        for(int i = 0; i < amount; i++){
            //new(parameter) is used to construct memory at a certain place opposed to allocating memory.
            new(component_data.data() + prior_end + (i * element_size)) Tcomp; 
        }
    }
    template<typename Tcomp>
    Tcomp& GetOrAllocate(Entity e_index) {
        assert(e_index < MAX_ENTITIES);
        size_t size = component_data.size() / element_size;
        if (size <= e_index)
            Allocate<Tcomp>(std::max<int64_t>(e_index - size + 1, 1));
        return Get<Tcomp>(e_index);
    }
    size_t GetElementSize(){
        return element_size;
    }
    size_t GetComponentDataSize(){
        return component_data.size()/element_size;
    } 
private:
    std::vector<std::byte> component_data;
    size_t element_size;
};


/// @brief 
/// Scene manager
struct SceneManager{
    std::vector<std::vector<bool>> entity_masks;
    std::vector<ComponentStorage> components;

    Entity CreateEntity(){
        assert(entity_masks.size() < MAX_ENTITIES);
        size_t size = entity_masks.size();
        entity_masks.emplace_back(std::vector<bool>{false});
        return size;
    }
    template<typename Tcomp>
    Tcomp& AddComponent(Entity e_index){
        assert(entity_masks.size() > 0);
        size_t component_index = Generator::TypeID<Tcomp>();

        //Check if component storage exists for component index, if not, then create it
        if(component_index >= components.size())
            components.insert(components.cend(), component_index - components.size() + 1, ComponentStorage());
        if(components[component_index].GetElementSize() == std::numeric_limits<size_t>::max())
            components[component_index] = ComponentStorage(Tcomp{});

        //Verify entity_mask of e_index size, if smaller than component index, increase by size of component index + 1
        auto& e_mask = entity_masks[e_index];
        if(e_mask.size() <= component_index)
            e_mask.resize(component_index + 1);
        //Set entity_mask to true
        e_mask[component_index] = true;
        return components[component_index].GetOrAllocate<Tcomp>(e_index);
    }

    template<typename Tcomp>
    Tcomp& GetComponent(Entity e_index){
        size_t component_index = Generator::TypeID<Tcomp>();
        assert(entity_masks[e_index].size() > component_index);
        assert(entity_masks[e_index][component_index]);
        return components[component_index].Get<Tcomp>(e_index);
    }

    template<typename Tcomp>
    bool HasComponent(uint16_t e_index){
        size_t component_index = Generator::TypeID<Tcomp>();
        assert(entity_masks.size() > e_index);
        assert(entity_masks[e_index].size() > component_index);
        return entity_masks[e_index][component_index];
    }
};
#endif