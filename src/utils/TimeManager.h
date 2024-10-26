#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include <vector>
#include <future>

class TimeManager {
public:
    TimeManager(bool useRealTime = false);

    template <typename Func>
    void add_task(Func&& func, int virtual_duration);

    template <typename Func>
    int run_single_task(Func&& func, int virtual_duration);

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
