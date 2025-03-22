#include <iostream>
#include <vector>

class String {
public:
    String() : buffer() {}
    String(int capacity) {
        buffer.reserve(capacity);
    }
    String(const char* cstr) {
        if (!cstr) throw std::invalid_argument("nullptr passed to String");
        size_t len = std::strlen(cstr);
        buffer.resize(len);
        std::memcpy(buffer.data(), cstr, len);
    }
    char& operator [](const int& idx) {
        if (!(idx < buffer.size())) throw std::out_of_range("out of str length");
        return buffer[idx];
    } 
    ~String() = default;
    String& operator =(const String& other) {
        size_t len = (other.buffer.size());
        if (len > buffer.size()) {
            buffer.resize(len);
        }
        std::memcpy(buffer.data(), other.buffer.data(), len);
        return *this;
    };
    String& operator =(const char* other) {
        size_t len = std::strlen(other);
        if (len > buffer.size()) {
            buffer.resize(len);
        }
        std::memcpy(buffer.data(), other, len);
        return *this;
    };
    
    //submit fn char buffer? 
    void push_back(const char& c) {
        buffer.emplace_back(c);
    }

    void replace(const int& idx, const char& replacement) {
        if (idx >= buffer.size()) throw std::out_of_range("index out of str range");
        buffer[idx] = replacement;
    }

    void print() {
        for (int i = 0; i < buffer.size(); i++) {
            std::cout << buffer[i];
        }
        std::cout << std::endl;
    }

private:
    std::vector<char> buffer;
};


int main() {
    String str(10);
    str = "Apples!!!";
    String s = "hello world";
    s.push_back('!');
    str.push_back('Z');
    str[0] = 'Z';
    str.replace(1, 'Z');
    s.print();
    str.print();
    std::cout << s[10] << str[1] << std::endl;
    return 0;
}