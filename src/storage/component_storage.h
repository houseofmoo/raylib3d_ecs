#pragma once

#include <vector>
#include <cstdint>
#include <cassert>
#include "data/entity.h"

namespace strg {
    template <typename T>
    struct ComponentStorage {

        std::vector<Entity> m_entity_id;
        std::vector<T> m_components;
        std::vector<int> m_indices; // m_indices[ENTITY_ID] returns ENTITY_INDEX into components/entity_id array

        void Add(Entity entity_id, const T& component) noexcept {
            // if this entity already has this component, do nothing
            if (Has(entity_id)) return;

            int index = m_components.size();
            m_entity_id.push_back(entity_id);
            m_components.push_back(component);

            if (entity_id >= m_indices.size()) {
                m_indices.resize(entity_id + 1, -1);
            }

            m_indices[entity_id] = index;
        }

        void Remove(Entity entity_id) noexcept {
            // if this entity does not have this component, do nothing
            if (!Has(entity_id)) return;

            int entity_index = m_indices[entity_id];
            int last_index = static_cast<int>(m_components.size()) - 1;
            Entity last_id = m_entity_id[last_index];
            
            // move last component to the new index
            m_entity_id[entity_index] = last_id;
            m_components[entity_index] = std::move(m_components[last_index]);
            m_indices[last_id] = entity_index;
     
            // delete entity components
            m_entity_id.pop_back();
            m_components.pop_back();
            m_indices[entity_id] = -1;
        }

        void Clear() noexcept {
            m_entity_id.clear();
            m_components.clear();
            m_indices.clear();
        }

        bool Has(Entity entity_id) const noexcept {
            return entity_id < m_indices.size() && m_indices[entity_id] >= 0;
        }

        T& Get(Entity entity_id) noexcept {
            assert(Has(entity_id) && "ComponentStorage::Get called on entity without this component");
            return m_components[m_indices[entity_id]];
        }

        const T& Get(Entity entity_id) const noexcept {
            assert(Has(entity_id) && "ComponentStorage::Get (const) called on entity without this component");
            return m_components[m_indices[entity_id]];
        }

        T* TryGet(Entity entity_id) noexcept {
            if (!Has(entity_id)) return nullptr;
            return &m_components[m_indices[entity_id]];
        }

        const T* TryGet(Entity entity_id) const noexcept {
            if (!Has(entity_id)) return nullptr;
            return &m_components[m_indices[entity_id]];
        }

        std::size_t Size() const noexcept {
            return m_components.size();
        }
    };
}