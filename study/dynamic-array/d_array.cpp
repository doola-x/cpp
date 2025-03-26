/*
======================================
Dynamic Array:
    - what state does the object have?
        - full, empty, partially filled, null
    - what invariants must always hold?
        - ptr to head of array (?) must always be valid and correct
        - size of array must never exceed capacity
    - what transitions can happen?
        - array is full and needs more space -- allocate new mem and copy over
        - array is copied -- copy ctor
        - array is moved -- move ctor
======================================
*/
#include <iostream>
template<typename T, uint64_t Capacity>
class DynamicArray {
static_assert(std::is_trivially_copyable_v<T>, "T must be trivially copyable");
static_assert(Capacity > 0, "d_array capacity must be > 0");
public:
    uint64_t size;

    DynamicArray() : size(0), capacity(Capacity), arr(new T[capacity]) {}
    DynamicArray(const DynamicArray& other) {
        // copy ctor
        size = other.size;
        capacity = other.capacity;

        arr = new T[other.size];
        for (size_t i = 0; i < other.size; i++) {
            arr[i] = other.arr[i];
        }
    }
    DynamicArray& operator=(const DynamicArray& other) {
        // copy assignment
        size = other.size;
        capacity = other.capacity;

        delete[] arr;
        arr = new T[other.size];
        for (size_t i = 0; i < other.size; i++) {
            arr[i] = other.arr[i];
        }

        return *this;
    }
    DynamicArray(DynamicArray&& other) {
        // move ctor
        size = other.size;
        capacity = other.capacity;
        
        arr = new T[other.size];
        for (size_t i = 0; i < other.size; i++) {
            arr[i] = std::move(other.arr[i]);
        }

        other.size = 0;
        other.capacity = 0;
        other.arr = nullptr;
    }
    DynamicArray& operator=(DynamicArray&& other) {
        // move assignment
        if (this == &other) return *this;
        size = other.size;
        capacity = other.capacity;
        delete[] arr;
        arr = other.arr;

        other.size = 0;
        other.capacity = 0;
        other.arr = nullptr;
        
        return *this;
    }
    ~DynamicArray() {
        // dtor
        printf("dtor\n");
        capacity = 0;
        size = 0;
        delete[] arr;
    }
    T& operator[](size_t idx) {
        if (idx >= capacity) throw std::out_of_range("invalid range");
        return *(arr+idx);
    }
    template<typename ...Args>
    void push_back(Args&& ...args) {
        if (size == capacity) {
            printf("alloc new mem\n");
            capacity *= 2;
            T* tmp = new T[capacity];
            std::memcpy(tmp, arr, sizeof(T) * size);
            delete[] arr;
            arr = tmp;
        }
        arr[size] = T(std::forward<Args>(args)...);
        size++;
    }

    void erase(size_t idx) {
        if (idx >= size) throw std::out_of_range("erase idx out of bounds");
        //pop T& at idx
        for (; idx < size - 1; idx++) {
            arr[idx] = arr[idx+1];
        }
        size--;
    }

private:
    uint64_t capacity;
    T* arr;
};


int main() {
    DynamicArray<int, 5> dn;
    for (int i = 0; i < 11; i++) {
        dn.push_back(i);
        std::cout << dn[i] << ", sizeof: " << sizeof(dn) << std::endl;
    }
    dn.erase(10);
    dn.erase(3);
    for (int i = 0; i < dn.size; i++) {
        std::cout << dn[i] << std::endl;
    }
    std::cout << "size: " << dn.size << std::endl;
    DynamicArray<int, 5> dn1(std::move(dn));
    std::cout << "size: " << dn.size << std::endl;
    return 0;
}