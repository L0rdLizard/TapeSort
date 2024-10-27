#include "TimeManager.h"
#include <thread>
#include <chrono>
#include <algorithm>

TimeManager::TimeManager(bool useRealTime) : useRealTime(useRealTime) {}

void TimeManager::run_tasks() {
    execution_time = 0;
    for (size_t i = 0; i < tasks.size(); ++i) {
        tasks[i].get();  // Ждем завершения каждой задачи

        if (useRealTime) {
            std::this_thread::sleep_for(std::chrono::seconds(virtual_times[i]));
        }

        execution_time = std::max(execution_time, virtual_times[i]);
    }
    global_execution_time += execution_time;
}

int TimeManager::get_execution_time() const {
    return execution_time;
}

int TimeManager::get_global_execution_time() const {
    return global_execution_time;
}
