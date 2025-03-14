#include "threadpool.hpp"

ThreadPool::ThreadPool(size_t num_threads) : is_running(true) {
    std::cout << "init " << num_threads << " threads..." << std::endl;
    threads.reserve(num_threads);
    for(int i = 0; i < num_threads; i++) {
        threads.emplace_back([this]() {
            while (true) {
                std::function<void()> task; 
                {
                    // acquire lock to prevent race conditions with task q
                    std::unique_lock<std::mutex> lock(_lock);
                    free_flag.wait(lock, [this]() {
                        // while tasks is empty or is running, sleep
                        return !tasks.empty() || !is_running;
                    });
                    if (!is_running && tasks.empty()) return;  

                    // move ownership of task to thread pool std::function
                    task = std::move(tasks.front());
                    tasks.pop();
                } // lock goes out of scope and is released
                task(); // execute newly aquired task
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    //destroy somethings?
    is_running = false;
    free_flag.notify_all(); // notify / wake all threads
    for (size_t i = 0; i < threads.size(); i++) {
        if (threads[i].joinable()) threads[i].join(); // join all threads to finish gracefully
    }
    std::cout << "eepy" << std::endl;
}

void ThreadPool::submit(std::function<void()> task) {
    {
        std::lock_guard<std::mutex> lock(_lock); // aquire simple lock guard
        tasks.emplace(task);
    }
    free_flag.notify_one(); // wake up one thread
}