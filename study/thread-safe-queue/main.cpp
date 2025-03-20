#include <atomic>
#include <vector>
#include "../thread-pool/threadpool.hpp"

struct ms_node {
    int value;
    std::atomic<ms_node*> next;
};

class ms_queue {
public:
    std::atomic<ms_node*> head;
    std::atomic<ms_node*> tail;

    void enqueue(ms_node* node) {
        ms_node* expected_head = nullptr; // potentially the head is null
        if (head.compare_exchange_weak(expected_head, node, std::memory_order_release)) { // if head is null and we store node there..
            tail.store(node, std::memory_order_release); // queue is empty, make tail node too
            return;
        }; // otherwise...

        ms_node* tail_u = tail.load(std::memory_order_acquire);
        while (true) {
            ms_node* expected = nullptr; // tail should be null
            if (tail_u->next.compare_exchange_weak(expected, node, std::memory_order_release)) { // if we succesfully store node..
                break; // some other threads may be waiting to update tail
            }
            ms_node* next = tail_u->next.load(std::memory_order_acquire); 
            tail_u = next ? next : tail.load(std::memory_order_acquire); // if tail->next is not null, update tail to new nxt ptr
                                                                        // otherwise, load tail again (ABA problem)
        }
        tail.compare_exchange_weak(tail_u, node, std::memory_order_release);
    }
    int dequeue() {
        while (true) {
            ms_node* head_u = head.load(std::memory_order_acquire);
            ms_node* tail_u = tail.load(std::memory_order_acquire);
            ms_node* next_u = head_u->next.load(std::memory_order_acquire);
    
            if (head_u == tail_u) {  
                if (next_u == nullptr) return -1;
                tail.compare_exchange_weak(tail_u, next_u, std::memory_order_release);
                continue;
            }
            if (head.compare_exchange_weak(head_u, next_u, std::memory_order_release)) {
                return next_u->value;
                // don't free yet
            }
        }
    }

    bool is_empty() {
        ms_node* head_u = head.load(std::memory_order_acquire);
        ms_node* tail_u = tail.load(std::memory_order_acquire);
        return head_u == tail_u;
    }
};

int main() {
    auto start_time = std::chrono::high_resolution_clock::now();
    ThreadPool pool(10);
    ms_queue q;

    for (int i = 0; i < 40; i++) {
        pool.submit([&]() {
            for (int j = 0; j < 40; j++) {
                q.enqueue(new ms_node{j});
            }
        });
    }

    // std::vector<std::thread> threads;
    // for (int i = 0; i < 10; ++i) {
    //     threads.emplace_back([&]() {
    //         for (int j = 0; j < 40; ++j) {
    //             q.enqueue(new ms_node{j});
    //         }
    //     });
    // }
    // for (auto& t : threads) t.join();

    std::cout << "passed!" << std::endl;
    // for (auto& t : pool.threads) {
    //     if (t.joinable()) t.join();
    // }
    while (true) {
        int val = q.dequeue();
        if (val == -1) break;
        // std::cout << "val:" << val << std::endl;
    }
    if (q.is_empty()) {
        std::cout << ":}" << std::endl;
    }
    auto end_time = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "1600 enqueues and dequeues w/ 10 threads..." << duration.count() << " ms"<< std::endl;
    start_time = std::chrono::high_resolution_clock::now();
    std::vector<int> v;
    for (int i = 0; i < 1600; i++) {
        v.emplace_back(i);
    }

    while (!v.empty()) {
        v.pop_back();
    }

    end_time = std::chrono::high_resolution_clock::now(); 
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    std::cout << "v 1600 enqueues and dequeues w/ 10 threads..." << duration.count() << " ms"<< std::endl;

    return 0;
}