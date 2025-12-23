#pragma once

#include <atomic>
#include <mutex>
#include <cstdint>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <tuple>
#include <cassert>

#include "data/entity.h"
#include "storage/component_storage.h"

namespace Storage {
    class Registry {
        private:
        struct IStorage {
            virtual ~IStorage() = default;
            virtual void Remove(Entity e) noexcept = 0;
            virtual void Clear() noexcept = 0;
        };

        template <typename T>
        struct TypedStorage : IStorage {
            ComponentStorage<T> storage;

            void Remove(Entity e) noexcept override {
                storage.Remove(e);
            }

            void Clear() noexcept override {
                storage.Clear();
            }
        };

        std::atomic<Entity> m_next_entity { 0 };
        std::mutex m_free_ids_mtx;
        std::vector<Entity> m_free_entity_ids;
        std::unordered_map<std::type_index, std::unique_ptr<IStorage>> m_storages;

        template <typename T>
        TypedStorage<T>& getTypedStorage() noexcept {
            const std::type_index type_index(typeid(T));
            auto it = m_storages.find(type_index);
            if (it == m_storages.end()) {
                auto ptr = std::make_unique<TypedStorage<T>>();
                auto* raw = ptr.get();
                m_storages.emplace(type_index, std::move(ptr));
                return *raw;
            }
            return *static_cast<TypedStorage<T>*>(it->second.get());
        }

        template <typename T>
        const TypedStorage<T>& getTypedStorage() const noexcept {
            const std::type_index type_index(typeid(T));
            auto it = m_storages.find(type_index);
            if (it == m_storages.end()) {
                // No storage yet => treat as empty
                // We cheat by creating a static empty storage for const access
                static const TypedStorage<T> empty{};
                return empty;
            }
            return *static_cast<const TypedStorage<T>*>(it->second.get());
        }

    public:
        // ---------- Entity API ----------

        Entity CreateEntity() noexcept {
            {   // attempt to reuse a old ID
                std::lock_guard lock(m_free_ids_mtx);
                if (!m_free_entity_ids.empty()) {
                    Entity id = m_free_entity_ids.back();
                    m_free_entity_ids.pop_back();
                    return id;
                }
            }

            // return a new ID
            return m_next_entity.fetch_add(1, std::memory_order_relaxed);
        }

        void DestroyEntity(Entity e) noexcept {
            // Remove this entity from all component storages
            for (auto& [_, storage] : m_storages) {
                storage->Remove(e);
            }
            
            {
                std::lock_guard lock(m_free_ids_mtx);
                m_free_entity_ids.push_back(e);
            }
        }

        void Reset() noexcept {
            // wipe all components everywhere
            for (auto& [_, storage] : m_storages) {
                storage->Clear();
            }

            // entity id allocator back to initial state
            m_next_entity.store(0, std::memory_order_relaxed);

            {
                std::lock_guard lock(m_free_ids_mtx);
                m_free_entity_ids.clear();
            }

            m_storages.clear();
        }
        
        // ---------- Component API ----------

        template <typename T, typename... Args>
        T& AddComponent(Entity e, Args&&... args) noexcept {
            auto& ts = getTypedStorage<T>();
            T component { std::forward<Args>(args)... };
            ts.storage.Add(e, component);
            return ts.storage.Get(e);
        }

        template <typename T>
        bool HasComponent(Entity e) const noexcept {
            const auto& ts = getTypedStorage<T>();
            return ts.storage.Has(e);
        }

        template <typename T>
        T& GetComponent(Entity e) noexcept {
            auto& ts = getTypedStorage<T>();
            return ts.storage.Get(e);
        }

        template <typename T>
        const T& GetComponent(Entity e) const noexcept {
            const auto& ts = getTypedStorage<T>();
            return ts.storage.Get(e);
        }

        template <typename T>
        T* TryGetComponent(Entity e) noexcept {
            auto& ts = getTypedStorage<T>();
            return ts.storage.TryGet(e);
        }

        template <typename T>
        const T* TryGetComponent(Entity e) const noexcept {
            const auto& ts = getTypedStorage<T>();
            return ts.storage.TryGet(e);
        }

        template <typename T>
        void RemoveComponent(Entity e) noexcept {
            auto& ts = getTypedStorage<T>();
            ts.storage.Remove(e);
        }

        // ---------- View (iteration) API ----------

        template <typename... Cs>
        class ViewRange {
            using First = std::tuple_element_t<0, std::tuple<Cs...>>;

            Registry* m_reg;
            ComponentStorage<First>* m_primary;

            struct Iterator {
                Registry* reg;
                ComponentStorage<First>* primary;
                std::size_t index;

                void advance() noexcept {
                    while (index < primary->m_entity_id.size()) {
                        Entity e = primary->m_entity_id[index];

                        // Check entity has all requested components Cs...
                        if ((reg->HasComponent<Cs>(e) && ...)) {
                            break;
                        }
                        ++index;
                    }
                }

                Entity operator*() const noexcept {
                    return primary->m_entity_id[index];
                }

                Iterator& operator++() noexcept {
                    ++index;
                    advance();
                    return *this;
                }

                bool operator!=(const Iterator& other) const noexcept {
                    return index != other.index;
                }
            };

        public:
            ViewRange(Registry* reg, ComponentStorage<First>* primary) noexcept
                : m_reg(reg), m_primary(primary) {}

            Iterator begin() noexcept {
                Iterator it{ m_reg, m_primary, 0 };
                it.advance();
                return it;
            }

            Iterator end() noexcept {
                return Iterator{ m_reg, m_primary, m_primary->m_entity_id.size() };
            }
        };

        template <typename... Cs>
        ViewRange<Cs...> View() noexcept {
            using First = std::tuple_element_t<0, std::tuple<Cs...>>;
            auto& ts = getTypedStorage<First>();
            return ViewRange<Cs...>{ this, &ts.storage };
        }

        template <typename... Cs>
        size_t Count() {
            size_t size = 0;
            for (auto e : View<Cs...>()) { ++size; }
            return size;
        }
    };
}
