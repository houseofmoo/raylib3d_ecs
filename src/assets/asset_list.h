#pragma once
#include <vector>
#include <type_traits>
#include <utility>

namespace asset {
    template <typename E>
    concept EnumType = std::is_enum_v<E>;
    
    template <typename T, EnumType E>
    class AssetList {
        private:
            using iterator = typename std::vector<T>::iterator;
            using const_iterator = typename std::vector<T>::const_iterator;

            std::vector<T> assets;

        public:
            const T& operator[](E type) const { return assets[static_cast<size_t>(type)]; }
            // turn off mutable access via [index]
            //T& operator[](E type) { return assets[static_cast<size_t>(type)]; }

            void Insert(E type, T asset) noexcept {
                const size_t idx = static_cast<size_t>(type);
                if (Size() <= idx) {
                    assets.resize(idx + 1);
                }
                assets[idx] = asset;
            }

            size_t Size() const noexcept { return assets.size(); }
            void Clear() noexcept { assets.clear(); }

            const_iterator begin() const { return assets.begin(); }
            const_iterator end() const { return assets.end(); }

            // consume via auto [beg, end] = AssetList.Iter();
            std::pair<iterator, iterator> Iter() noexcept { 
                return std::pair{ assets.begin(), assets.end() };
            }

            // consume via auto [beg, end] = AssetList.Iter();
            std::pair<const_iterator, const_iterator> Iter() const noexcept { 
                return std::pair{ assets.cbegin(), assets.cend() };
            }
    };
}