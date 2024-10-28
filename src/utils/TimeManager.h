#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <vector>
#include <future>
#include <algorithm>
#include <thread>
#include "Task.h"

class TimeManager {
public:
    TimeManager(bool useRealTime);

    template <typename T>
    void add_task(Task<T>&& task) {
        tasks.push_back(std::move(task.future));
        virtual_times.push_back(task.delay);
    }

    template <typename T>
    T run_single_task(Task<T>&& task) {
        if constexpr (!std::is_void_v<T>) {
            T result = task.future.get();
            execution_time = task.delay;
            global_execution_time += execution_time;

            if (useRealTime) {
                std::this_thread::sleep_for(std::chrono::milliseconds(task.delay));
            }

            return result;
        } else {
            task.future.get();
            execution_time = task.delay;
            global_execution_time += execution_time;

            if (useRealTime) {
                std::this_thread::sleep_for(std::chrono::milliseconds(task.delay));
            }
        }
    }


    void run_tasks();

    int get_execution_time() const;
    int get_global_execution_time() const;
    const std::vector<int>& get_results() const;

private:
    std::vector<std::future<int>> tasks;
    std::vector<int> virtual_times;
    std::vector<int> results;
    int execution_time = 0;
    int global_execution_time = 0;
    bool useRealTime;
};

#endif // TIMEMANAGER_H
