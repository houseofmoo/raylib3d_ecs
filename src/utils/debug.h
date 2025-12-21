#pragma once

#ifdef DEBUG
    #include <unordered_map>
    #include <iostream>
    #include <mutex>
    #include <sstream>
    #include "data/player/player.h"

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

    #if PROFILER_ENABLED
        namespace Profiler {
            struct ProfileSample {
                std::string_view name;
                long long microseconds;
            };

            struct ProfileResults {
                long long total_time;
                long long max_time;
                uint64_t sample_with_max_time;
                long long min_time;
                uint64_t num_samples;
            };

            inline std::unordered_map<std::string_view, ProfileResults> g_samples;

            struct ProfileScope {
                using clock = std::chrono::high_resolution_clock;
                std::string_view name;
                clock::time_point start;

                explicit ProfileScope(std::string_view n)
                    : name(n), start(clock::now()) {}

                ~ProfileScope() {
                    if (!data::player::g_player.profiler_enabled) return;
                    auto end = clock::now();
                    auto us = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

                    if (!g_samples.contains(name)) {
                        g_samples.emplace(name, ProfileResults{0, 0, 0, 1000, 0});
                    }

                    g_samples[name].total_time += us;
                    g_samples[name].num_samples += 1;

                    if (g_samples[name].max_time < us) {
                        g_samples[name].max_time = us;
                        g_samples[name].sample_with_max_time = g_samples[name].num_samples;
                    }
                
                    if (g_samples[name].min_time > us) {
                        g_samples[name].min_time = us;
                    }
                }
            };


            inline void PrintProfiler() {
                PRINT(" ");
                for (auto& [k, v] : g_samples) {
                    PRINT(k);
                    PRINT("\tavg time:     ", (double)v.total_time / (double)v.num_samples, " us");
                    PRINT("\tmax time:     ", v.max_time, " us");
                    PRINT("\tidx max time: ", v.sample_with_max_time);
                    PRINT("\tmin time:     ", v.min_time, " us");
                    PRINT("\tsamples:      ", v.num_samples);
                    PRINT(" ");
                }
            }

            inline void ClearProfiler() {
                g_samples.clear();
            }
        }

        #define PROFILE_SCOPE(name) Profiler::ProfileScope _prof_scope_##__LINE__{name}
        #define PROFILER_PRINT Profiler::PrintProfiler()
        #define PROFILER_CLEAR Profiler::ClearProfiler()
    #else
        // disable profiler when DEBUG && !PROFILER_ENABLED
        #define PROFILE_SCOPE(name)
        #define PROFILER_PRINT
        #define PROFILER_CLEAR
    #endif
#else
    // disable all debug when !DEBUG
    #define PRINT(...)
    #define ERROR(...)
    #define LOG(...)
    #define PROFILE_SCOPE(name)
    #define PROFILER_PRINT
    #define PROFILER_CLEAR
#endif