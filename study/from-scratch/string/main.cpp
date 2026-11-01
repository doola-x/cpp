#include "string.hpp"
#include <iostream>

void use(const str::string& s) {
    std::cout << s.get() << std::endl;
}

int main() {
    const char* s = new char[12];
    s = "hello worlds";
    std::string s_np(nullptr, 0);
    str::string s_ptr(s, 24);
    str::string s_ptr2("apples", 100);
    s_ptr = std::move(s_ptr2);
    s_ptr.append('!');
    use(s_ptr);
    return 0;
}
