#include <cstddef>
#include <cstring>
#include <iostream>
namespace str {
class string {
    public:
        string() : data(nullptr) {}
        string(const char* _data, std::size_t _capacity) {
            size = std::strlen(data);
            capacity = _capacity;
            data = new char[capacity];
            std::memcpy(data, _data, size+1);
        }
        
        string& operator=(string&& other) {
            std::cout << "moving" << '\n';
            if (this == &other) {
                return *this;
            }
            
            delete[] data;

            data = other.data;
            size = other.size;
            capacity = other.capacity;
            
            other.size = 0;
            other.capacity = 0;
            other.data = nullptr;
            
            return *this;
        }

        ~string() {
            delete[] data;
        }

        string& operator=(const string& other) = delete;

        char* get() const {
            return data;
        }

        bool append(const char c) {
            if (size + 1 > capacity) {
                return false;
            }

            data[++size] = c;
            data[size] = '\0';
            return true;
        }

    private:
        char* data;
        std::size_t size;
        std::size_t capacity;
};
}
