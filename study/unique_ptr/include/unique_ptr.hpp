#include <utility>
namespace ptr {
template<typename T>
class unique_ptr {
public:
    unique_ptr() : value(nullptr) {}
    explicit unique_ptr(T* p) : value(p) {
    }
    ~unique_ptr() {
        delete value;
    }
    unique_ptr(unique_ptr&& other) noexcept : value(other.value) {
        other.value = nullptr;
    }
    unique_ptr& operator=(unique_ptr&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete value;
        value = other.value;
        other.value = nullptr;
        return *this;
    }

    unique_ptr(const unique_ptr&) = delete; 
    unique_ptr& operator=(const unique_ptr&) = delete;

    T* operator->() const {
        return value;
    }
    T* get() const {
        return value;
    }
    T& operator*() const {
        return *value;
    }

    T* release() {
        T* tmp = value;
        value = nullptr;
        return tmp;
    }
    void reset(T* p = nullptr) {
        T* tmp = value;
        value = p;
        delete tmp;
    }
private:
    T* value = nullptr;
};


template<typename T, typename... Args>
unique_ptr<T> make_unique(Args&&... args) {
    return unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}