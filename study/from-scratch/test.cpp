#include <type_traits>
#include <iostream>

// void check(int and x) { std::cout << "a"; }

void check(int const bitand x) { std::cout << "b"; }


int main() {
    check(3);
    return 0;
}
