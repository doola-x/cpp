#include <iostream>

class String {
public:
    size_t capacity;
    size_t size;

    String(size_t _size) : size(_size) {
        buffer = new char[size];
        capacity = size * 2; 
    }

    String(size_t _capacity, char* _buffer) {
        if (_capacity < strlen(buffer)) {
            throw std::out_of_range("string is too long you stupid bitch");
        }
        buffer = _buffer;
        capacity = _capacity;
        size = _capacity;
    }

    void push_back(char&& c) {
        if (size == capacity) {
            // more mem
        }
        // push back char
        std::cout << "curr size: " << this->size << std::endl;
        buffer[size+1] = c;
        size++;
    }

    void print() {
        for (size_t i = 0; i <= size; i++) {
            std::cout << buffer[i];
        }

        std::cout << std::endl;
    }

    ~String() {
        delete[] buffer;
    }

private:
    char* buffer;
};


int main() {
    String s(2);
    s.push_back('a');
    s.push_back('b');
    s.push_back('c');
    s.push_back('c');
    s.push_back('c');
    s.push_back('c');
    s.print();
    std::cout << "size: " << s.size << std::endl;

    return 0;
}