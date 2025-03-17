#include "threadpool.hpp"
#include <chrono>
#include <atomic>
#include <fstream>
#include <string>

std::queue<std::string> logs;
std::mutex log_m;
std::condition_variable log_cv;
std::atomic<int> active_c{0};
bool done = false;

void logger() {
    while (!done) {
        {
            std::unique_lock<std::mutex> lock(log_m);
            log_cv.wait(lock, [] {
                return !logs.empty() || ((active_c.load(std::memory_order_acquire) == 0 && done));
            });

            while (!logs.empty()) {
                std::cout << logs.front();
                logs.pop();
            }

            if (active_c.load(std::memory_order_acquire) == 0 && logs.empty()) break;
        }
    }
}
void print_async(int i) {
    std::ostringstream oss;
    oss << "this is task " << i << "\n";
    {
        std::lock_guard<std::mutex> lock(log_m);
        logs.push(oss.str());
    }
    log_cv.notify_one();
}
void print_task(int i) {
    std::cout << "this is task " << i << std::endl;
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    std::thread log_thread(logger);
    ThreadPool pool(10);
    for (int i = 0; i < 100000; i++) {
        active_c.fetch_add(1, std::memory_order_relaxed);
        pool.submit([i]() {
            print_async(i);
            active_c.fetch_sub(1, std::memory_order_release);
            log_cv.notify_one();
        });
    }
    while (active_c.load(std::memory_order_acquire) > 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // busy-wait until all active tasks complete
    }
    {
        std::lock_guard<std::mutex> lock(log_m);
        done = true;
    }
    log_cv.notify_one();
    log_thread.join();
    auto end_time = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "10 threads, 100000 tasks: " <<  duration.count() << " ms" << std::endl;;
    std::cout << "main program exiting..." << std::endl;
    return 0;
}