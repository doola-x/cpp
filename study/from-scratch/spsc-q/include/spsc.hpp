#include <atomic>
#include <cstddef>
#include <optional>

namespace spsc {
constexpr int MAX_CAPACITY = 2048; 

template<typename T, std::size_t Capacity> 
class Queue {
    public:
        Queue() = default;
        ~Queue() {
            delete[] buffer_;
        }

        bool push(const T* item) {
            // acquire a load
            auto slow = consumer_.load(std::memory_order_acquire);
            auto fast = producer_.load(std::memory_order_acquire);
           
            // if number of objects exceeds cap ban it
            if ((fast - slow) >= capacity_) {
                return false;
            }
            
            // if not we can push
            buffer_[fast % capacity_] = *item; 
            producer_.fetch_add(1, std::memory_order_release);
            return true;
        }

        std::optional<T> pop() {
            // acquire a load
            auto slow = consumer_.load(std::memory_order_acquire);
            auto fast = producer_.load(std::memory_order_acquire);

            // if pointers are equal, the queue is empty
            if ((fast - slow) == 0) {
                return std::nullopt;
            }
            
            T item = std::move(buffer_[slow % capacity_]);
            consumer_.fetch_add(1, std::memory_order_release);
            return item;
        }
    private:
        alignas(64) std::atomic<uint64_t> consumer_ {0};
        alignas(64) std::atomic<uint64_t> producer_ {0};
        std::size_t capacity_ {0};
        T* buffer_ = new T[Capacity];
};
}

