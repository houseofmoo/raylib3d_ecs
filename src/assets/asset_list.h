#pragma once
#include <vector>

namespace asset {
    template <typename E>
    concept EnumType = std::is_enum_v<E>;
    
    template <typename T, EnumType E>
    class AssetList {
        private:
            std::vector<T> assets;

        public:
            void Clear() noexcept { assets.clear(); }

            T& Get(E type) { return assets[static_cast<int>(type)]; }

            void Set(E type, T asset) noexcept {
                if (assets.size() <= static_cast<size_t>(type)) {
                    assets.resize(static_cast<int>(type) + 1);
                }
                assets[static_cast<size_t>(type)] = asset;
            }

            auto Iter() noexcept {
                return std::pair { assets.begin(), assets.end() };
            }

    };
}