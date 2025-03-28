#include <iostream>

template<typename T, size_t Capacity>
class Vector {
public:
    // ctor
    Vector() = default;
    // dtor
    ~Vector() {
        delete[] new_heap;
    }

    Vector(const Vector& other) {
        printf("copied ctor\n");
        count = other.count; // copy count
        heap_capacity = other.heap_capacity; // copy heap capacity
        for (size_t i = 0; i < std::min(count, Capacity); i++) { // stack may hold <= capacity, need only init obj
            stack[i] = other.stack[i]; // move stack memory from other object to this
        }
        if (heap_capacity > 0) {
            new_heap = new T[heap_capacity];
            std::copy(other.new_heap, other.new_heap + (count - Capacity), new_heap);
        }
        else {
            new_heap = nullptr;
        }
    }

    Vector(Vector&& other) noexcept {
        printf("move ctor\n");
        count = other.count;
        heap_capacity = other.heap_capacity;
        for (size_t i = 0; i < std::min(count, Capacity); i++) {
            stack[i] = std::move(other.stack[i]);
        }
        new_heap = other.new_heap;

        other.count = 0;
        other.heap_capacity = 0;
        other.new_heap = nullptr;
    }

    Vector& operator=(const Vector& other) {
        printf("copied assign\n");
        if (this == &other) return *this;
        delete[] new_heap;

        count = other.count;
        heap_capacity = other.heap_capacity;
        for (size_t i = 0; i < std::min(count, Capacity); i++) {
            stack[i] = other.stack[i];
        }
        
        if (heap_capacity > 0) {
            new_heap = new T[heap_capacity];
            std::copy(other.new_heap, other.new_heap + (count - Capacity), new_heap);
        } else {
            new_heap = nullptr;
        }

        return *this;
    }

    Vector& operator=(Vector&& other) {
        printf("move assign\n");
        if (this == &other) return *this;

        delete[] new_heap;
        count = other.count;
        heap_capacity = other.heap_capacity;
        for (size_t i = 0; i < std::min(count, Capacity); i++) {
            stack[i] = std::move(other.stack[i]);
        }
        new_heap = other.new_heap;

        other.count = 0;
        other.heap_capacity = 0;
        other.new_heap = nullptr;

        return *this;
    }

    T& operator[](size_t idx) {
        if (idx >= count) throw;
        if (idx < Capacity) {
            return stack[idx];
        }
        return new_heap[idx - Capacity];
    }

    template<typename ...Args>
    void push_back(Args&& ...args) {
        if (count < Capacity) {
            stack[count] = T(std::forward<Args>(args)...);
            count++;
            return;
        }
        if (count == Capacity) {
            // get new mem
            heap_capacity = (count) * 2;
            new_heap = new T[heap_capacity];
        } else if (count == Capacity + heap_capacity) {
            // get more mem and copy
            heap_capacity = (count) * 2;
            T* temp = new T[heap_capacity];
            std::copy(new_heap, new_heap + (count - Capacity), temp);
            delete[] new_heap;
            new_heap = temp;
        }
        new_heap[count - Capacity] = T(std::forward<Args>(args)...);
        count++;
    }

    void pop_back() {
        if (count == 0) throw;
        if (count > Capacity) {
            new_heap[count - Capacity - 1].~T();
        } else {
            stack[count - 1].~T();
        }
        count--;
    }

    size_t size() const {
        return count;
    }
private:
    size_t count = 0;
    size_t heap_capacity = 0;
    T stack[Capacity];
    T* new_heap = nullptr;
};

int main() {
    Vector<int, 2> v;
    for (int i = 0; i < 10; i++) {
        v.push_back(i);
    }
    Vector<int, 2> v0(v);
    v.push_back(11);
    v0 = std::move(v);
    v = std::move(v0);

    for (int i = 0; i < v.size(); i++) {
        std::cout << v[i] << std::endl;
    }
    for (int i = 0; i < v0.size(); i++) {
        std::cout << v0[i] << std::endl;
    }
    
    Vector<std::string, 2> v1;
    v1.push_back("hello");
    v1.push_back(", world");
    v1.push_back("!");
    for (int i = 0; i < v1.size(); i++) {
        std::cout << v1[i];
    }
    std::cout << std::endl;
    return 0;
}