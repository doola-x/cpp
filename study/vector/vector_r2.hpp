
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <stdexcept>

namespace r2 {

template<typename T>
class Vec {
public:
    Vec() : size_(0), capacity_(16) 
    {
        buffer_ = new T[capacity_];
    }
    ~Vec() 
    {
        delete[] buffer_;
    }

    Vec(const Vec& other) = delete;
    Vec& operator=(const Vec& other) = delete;
    
    Vec(Vec&& other) noexcept 
        : size_(other.size_)
        , buffer_(other.buffer_)
        , capacity_(other.capacity_)
    {
        other.size_ = 0;
        other.capacity_ = 0;
        other.buffer_ = nullptr;
    }

    Vec& operator=(Vec&& other) noexcept
    {
        if (this == &other) {
            return *this;
        }
        delete[] buffer_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        buffer_ = other.buffer_;
        other.buffer_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;

        return *this;
    }

    T& operator[](std::size_t i) {
        if (i >= size_) {
            throw std::out_of_range("ayo!");
        }

        return buffer_[i];
    }

    void push_back(const T& value) {
        if (size_ < capacity_) {
            buffer_[size_] = value;
            size_++;
            return;
        }
        // more mem
        capacity_ *= 2;
        T* new_buff = new T[capacity_];
        for (std::size_t i = 0; i < size_; i++) {
            new_buff[i] = std::move(buffer_[i]);
        }
        delete[] buffer_;
        buffer_ = new_buff;
        buffer_[size_] = value;
        size_++;
    }

    void push_back(T&& value) {
        if (size_ < capacity_) {
            buffer_[size_] = std::move(value);
            size_++;
            return;
        }
        // more mem
        capacity_ *= 2;
        T* new_buff = new T[capacity_];
        for (std::size_t i = 0; i < size_; i++) {
            new_buff[i] = std::move(buffer_[i]);
        }
        delete[] buffer_;
        buffer_ = new_buff;
        buffer_[size_] = std::move(value);
        size_++;
    }

    std::size_t size() const
    {
        return size_;
    }
    std::size_t capacity() const
    {
        return capacity_;
    }
    
private:
    T* buffer_ = nullptr;
    std::size_t size_;
    std::size_t capacity_;
};

}
