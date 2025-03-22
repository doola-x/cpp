#include <atomic>
#include <thread>

template<typename T, size_t Capacity>
class pub_sub {
public:
    pub_sub() : head(0), tail(0);
    ~pub_sub();
    operator= pub_sub() = delete;
    operator move pub_sub() = delete;

    bool enqueue(const T& obj) {
        size_t tail_unsafe = tail.load(std::memory_order_acquire);
        size_t tail_n = (tail_unsafe + 1) % Capacity;
        if (tail_n == head.load(std::memory_order_acquire)) {
            return false; // buffer full
        }

        buffer[tail_unsafe] = obj;
        tail.store(tail_n, std::memory_order_release);
        return true;
    }

    bool dequeue(T& out) {
        
    }

private:
    std::array<size_t, Capacity> buffer;
    std::atomic<size_t> head;
    std::atomic<size_t> tail;
};