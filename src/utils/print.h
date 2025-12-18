#pragma once
    #include <iostream>
    #include <mutex>
    #include <sstream>

#ifdef DEBUG
    #define PRINT(...) Print::Print(__VA_ARGS__);
    #define ERROR(...) Print::Error(__VA_ARGS__);
    #define LOG(...) Print::Log(__VA_ARGS__);

    namespace Print {
        inline std::mutex print_mtx;

        template <typename... Args>
        inline void Print(Args&&... args) noexcept {
            std::lock_guard<std::mutex> lock(print_mtx);
            (std::cout << ... << args) << std::endl;
        }

        template <typename... Args>
        inline void Error(Args&&... args) noexcept {
            std::lock_guard<std::mutex> lock(print_mtx);
            (std::cerr << ... << args) << std::endl;
        }

        template <typename... Args>
        inline void Log(Args&&... args) noexcept {
            std::lock_guard<std::mutex> lock(print_mtx);
            (std::clog << ... << args) << std::endl;
        }
    }
#else
    // disable prints in release mode
    #define PRINT(...)
    #define ERROR(...)
    #define LOG(...)
#endif