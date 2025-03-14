#include <new>
#include <cstddef>
#include <cstring>
#include <stdexcept>

class Stack {
	static constexpr std::size_t STACK_SIZE = 2048;
	alignas(std::max_align_t) char stack[STACK_SIZE];
	std::size_t offset = 0;

public:
	template<typename T, typename... Args>
	T* allocate(Args&&... args) {
		if (offset + sizeof(T) > STACK_SIZE) throw std::bad_alloc();
		void* ptr = stack + offset;
		offset += sizeof(T);
		return new (ptr) T(std::forward<Args>(args)...);
	}
	template<typename T, typename... Args>
	void deallocate(T* obj) {
		if (obj) obj->~T();
		void* ptr = stack + offset;
		offset -= sizeof(T);
	}
};