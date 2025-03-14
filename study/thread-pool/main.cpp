#include "threadpool.hpp"
#include <chrono>

void print_task(int id) {
    std::cout << "thread " << id << " is running on thread " << std::this_thread::get_id() << std::endl;
}

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    ThreadPool pool(10);
    for (int i = 0; i < 100000; i++) {
        pool.submit([i]() {
            print_task(i);
        });
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto end_time = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "10 threads, 100000 tasks: " <<  duration.count() << " ms" << std::endl;;
    std::cout << "main program exiting..." << std::endl;
    return 0;
}