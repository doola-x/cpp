#include <utility>

namespace ptr {

template<typename T>
class shared_ptr {
public:
    // this is not thread safe, there is no atomic operations or lock around the ref count
    explicit shared_ptr(T* value) 
        : value_(value), ref_(new int(1)) {}

    ~shared_ptr() {
        if (! ref_) return;
        (*ref_)--;
        if ((*ref_) == 0) {
            delete value_;
            delete ref_;
        }
    }

    shared_ptr(const shared_ptr& other) 
        : value_(other.value_)
        , ref_(other.ref_)
    {
        (*ref_)++;
    }
    shared_ptr(shared_ptr&& other) 
        : value_(other.value_)
        , ref_(other.ref_)
    {
        other.value_ = nullptr;
        other.ref_ = 0;
    }

    shared_ptr& operator=(const shared_ptr& other) {
        if (this == &other) {
            return (*this);
        }
        (*ref_)--;
        value_ = other.value_;
        ref_ = other.ref_;
        (*ref_)++;
        return *this;
    }
    shared_ptr& operator=(shared_ptr&& other) {
        if (this == &other) {
            return (*this);
        }
        (*ref_)--;
        value_ = other.value_;
        ref_ = other.ref_;

        other.value_ = nullptr;
        other.ref_ = nullptr;
        return *this;
    }
private:
    T* value_;
    int* ref_;
    
};
}