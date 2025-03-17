#include <queue>
#include <thread>
#include <vector>
#include <iostream>

class ThreadPool {
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();
    std::vector<std::thread> threads;
    void submit(std::function<void()>);

private:
    bool is_running;
    std::mutex _lock;
    std::condition_variable free_flag;
    std::queue<std::function<void()>> tasks;
};