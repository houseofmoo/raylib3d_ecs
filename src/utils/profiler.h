#pragma once

#include <unordered_map>
#include <chrono>
#include <string_view>
#include <vector>
#include "utils/print.h"

namespace profiler {
    #ifdef PROFILER_ENABLED
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

    // Simple global storage for now
    inline std::unordered_map<std::string_view, ProfileResults> g_samples;

    struct ProfileScope {
        using clock = std::chrono::high_resolution_clock;
        std::string_view name;
        clock::time_point start;

        explicit ProfileScope(std::string_view n)
            : name(n), start(clock::now()) {}

        ~ProfileScope() {
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

    #define PROFILE_SCOPE(name) ProfileScope _prof_scope_##__LINE__{name}

    inline void Print() {
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

    inline void Clear() {
        g_samples.clear();
    }
    #else
    #define PROFILE_SCOPE(name)
    inline void PrintProfile() {}
    #endif
}