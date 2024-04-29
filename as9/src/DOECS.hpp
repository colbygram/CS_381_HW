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

extern size_t globalComponentCounter;

template<typename T>
size_t GetComponentID(T reference = {}) {
    static size_t id = globalComponentCounter++;
    return id;
}

using Entity = uint8_t; 

//ComponentStorage is a container for varying components
//Uses std::byte so that it is ambiguous what data can be stored in it
//
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
#endif