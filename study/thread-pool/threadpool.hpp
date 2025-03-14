#include <queue>
#include <thread>
#include <vector>
#include <iostream>

class ThreadPool {
public:
    ThreadPool(size_t num_threads);
    ~ThreadPool();
    void submit(std::function<void()>);

private:
    bool is_running;
    std::mutex _lock;
    std::vector<std::thread> threads;
    std::condition_variable free_flag;
    std::queue<std::function<void()>> tasks;
};