#include <iostream>
#include "unique_ptr.hpp"

int main() {
    int* x = new int(42);
    UniquePointer<int> uptr(x);
    std::cout << uptr.get() << std::endl;
    UniquePointer<int> u_ptr_move = std::move(uptr);
    u_ptr_move = std::move(uptr);
    std::cout << u_ptr_move.get() << std::endl;
    volatile int* f = u_ptr_move.get();
}
