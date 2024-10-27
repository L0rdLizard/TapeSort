#ifndef TASK_H
#define TASK_H

#include <future>

template <typename T>
struct Task {
    std::future<T> future;
    int delay;

    Task(std::future<T> future, int delay) : future(std::move(future)), delay(delay) {}
};

#endif // TASK_H