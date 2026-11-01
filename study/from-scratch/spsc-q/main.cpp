#include "include/spsc.hpp"
#include <functional>
#include <iostream>

struct Task {
    int id;
    std::function<void()> task;
};

int main() {
    spsc::Queue<Task, 1024> q;

    q.push(new Task(0, [&](){ std::cout << "ello: "; }));
    auto fetch = q.pop();
    if (fetch.has_value()) {
        std::cout << "hurray!";
    }

    return 1;
}
