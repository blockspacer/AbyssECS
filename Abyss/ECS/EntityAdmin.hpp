#pragma once

#include "Entity.hpp"
#include "TypeID.hpp"

#include <functional>
#include <vector>
#include <array>
#include <type_traits>

namespace Abyss {
    namespace ECS {
        class EntityAdmin;
    }
}

class Abyss::ECS::EntityAdmin {
protected:
    using EntityArray           = std::vector<Entity>;
    using EntityDescriptor      = EntityArray::iterator;
    using EntityDescriptorArray = std::vector<EntityDescriptor>;
    using ComponentPool         = std::pair<std::vector<void *>, std::function<void(void *)>>;
    using ComponentPoolArray    = std::vector<ComponentPool>;

    EntityArray           m_entities{};
    EntityDescriptorArray m_deadEntities{};
    ComponentPoolArray    m_componentPools{};
    std::size_t           m_maxID{0};

public:
    virtual ~EntityAdmin();

    [[nodiscard]] auto createEntity() -> EntityDescriptor;
    auto               destroyEntity(EntityDescriptor &entity) -> void;

    template <typename T, typename... Ts>
    auto addComponent(EntityDescriptor &entity, Ts &&... ts) -> T &;

    template <typename T>
    [[nodiscard]] auto hasComponent(EntityDescriptor &entity) -> bool;

    template <typename... Ts>
    [[nodiscard]] auto hasComponents(EntityDescriptor &entity) -> bool;

	template <typename T>
    [[nodiscard]] auto getComponent(EntityDescriptor &entity) -> T &;

    template <typename... Ts>
    [[nodiscard]] auto getComponents(EntityDescriptor &entity) -> std::tuple<Ts &...>;

    template <typename T>
    auto deleteComponent(EntityDescriptor &entity) -> void;
};

inline Abyss::ECS::EntityAdmin::~EntityAdmin()
{
    for (const auto &[pool, deleter] : m_componentPools) {
        if (!deleter)
            continue;

        for (const auto &component : pool)
            deleter(component);
    }
}

inline auto Abyss::ECS::EntityAdmin::createEntity() -> EntityDescriptor
{
    if (m_deadEntities.size() > 0) {
        auto it{m_deadEntities.begin()};
        auto val{*it};
        m_deadEntities.erase(it);
        return val;
    }

    m_entities.emplace_back(0);
    return m_entities.end() - 1;
}

inline auto Abyss::ECS::EntityAdmin::destroyEntity(EntityDescriptor &entity) -> void
{
    *entity = 0;
    m_deadEntities.emplace_back(entity);
}

template <typename T, typename... Ts>
inline auto Abyss::ECS::EntityAdmin::addComponent(EntityDescriptor &entity, Ts &&... ts) -> T &
{
    auto &&[id, bitID]{TypeID::info<T>()};
    auto &bitfield{*entity};
    auto  index{static_cast<unsigned long long>(std::distance(m_entities.begin(), entity))};

    if (id >= m_maxID) {
        m_componentPools.resize(id + 1);
        m_componentPools[id].second = [](void *ptr) {
            delete static_cast<T *>(ptr);
        };
        m_maxID = id + 1;
    }

    auto &&[pool, deleter]{m_componentPools[id]};
    if (pool.size() <= index)
        pool.resize(index + 1);

    auto &component{pool[index]};
    if (component != nullptr)
        new (component) T{std::forward<Ts>(ts)...};
    else
        component = new T{std::forward<Ts>(ts)...};

    bitfield |= bitID;
    return *static_cast<T *>(component);
}

template <typename T>
inline auto Abyss::ECS::EntityAdmin::hasComponent(EntityDescriptor &entity) -> bool
{
    auto &bitfield{*entity};
    auto  bitID{TypeID::info<T>().second};

    return ((bitfield & bitID) == bitID);
}

template <typename... Ts>
inline auto Abyss::ECS::EntityAdmin::hasComponents(EntityDescriptor &entity) -> bool
{
	std::array<bool, sizeof...(Ts)> array{hasComponent<Ts>(entity)...};
    for (auto val : array)
		if (!val)
			return false;
	return true;
}

template <typename T>
inline auto Abyss::ECS::EntityAdmin::getComponent(EntityDescriptor &entity) -> T&
{
	if (!hasComponent<T>(entity))
		throw std::runtime_error{std::string{"Couldn't get component "} + typeid(T).name()};

    auto &&[id, bitID]{TypeID::info<T>()};
    auto  index{static_cast<unsigned long long>(std::distance(m_entities.begin(), entity))};
    auto &&[pool, deleter]{m_componentPools[id]};
    return *static_cast<T *>(pool[index]);
}

template <typename... Ts>
inline auto Abyss::ECS::EntityAdmin::getComponents(EntityDescriptor &entity) -> std::tuple<Ts &...>
{
	std::tuple<Ts &...> tuple{getComponent<Ts>(entity)...};
	return tuple;
}

template <typename T>
auto Abyss::ECS::EntityAdmin::deleteComponent(EntityDescriptor &entity) -> void
{
    auto &&[id, bitID]{TypeID::info<T>()};
    auto &bitfield{*entity};

    bitfield &= ~bitID;
}
