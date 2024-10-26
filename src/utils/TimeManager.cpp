#include "TimeManager.h"
#include <algorithm>
#include <thread>

TimeManager::TimeManager(bool useRealTime = false) : useRealTime(useRealTime) {}

template <typename Func>
void TimeManager::add_task(Func&& func, int virtual_duration) {
    tasks.push_back(std::async(std::launch::async, std::forward<Func>(func)));
    virtual_times.push_back(virtual_duration);
}

void TimeManager::run_tasks() {
    execution_time = 0;

    for (size_t i = 0; i < tasks.size(); ++i) {
        results.push_back(tasks[i].get());

        if (useRealTime) {
            std::this_thread::sleep_for(std::chrono::seconds(virtual_times[i]));
        }

        execution_time = std::max(execution_time, virtual_times[i]);
    }

    global_execution_time += execution_time;
}

template <typename Func>
int TimeManager::run_single_task(Func&& func, int virtual_duration) {
    auto result = std::async(std::launch::async, std::forward<Func>(func)).get();

    execution_time = virtual_duration;
    global_execution_time += execution_time;

    if (useRealTime) {
        std::this_thread::sleep_for(std::chrono::seconds(virtual_duration));
    }

    results.push_back(result);
    return result;
}

int TimeManager::get_execution_time() const {
    return execution_time;
}

int TimeManager::get_global_execution_time() const {
    return global_execution_time;
}

const std::vector<int>& TimeManager::get_results() const {
    return results;
}
