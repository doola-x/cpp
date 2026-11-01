#include <stdexcept>
template<typename T>
class UniquePointer {
public:
    UniquePointer() : m_ptr(nullptr) {};
    UniquePointer(T* ptr) : m_ptr(ptr) {};
 
    ~UniquePointer() {
        delete m_ptr;
    }   

    UniquePointer(UniquePointer&& other) noexcept 
        : m_ptr(other.m_ptr) 
    {
        other.m_ptr = nullptr;
    }

    UniquePointer& operator=(UniquePointer&& other) noexcept{
        if (this == &other) {
            return *this;
        }

        delete m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = nullptr;
        return *this;
    }
    
    UniquePointer(const UniquePointer& other) = delete;
    UniquePointer& operator=(const UniquePointer& other) = delete;

    T* get() const {
        return m_ptr;
    }

    T& operator->() const {
        if (!m_ptr) {
            throw std::logic_error("null ptr deref");
        }
        return *m_ptr;
    }

private:
    T* m_ptr;
};
