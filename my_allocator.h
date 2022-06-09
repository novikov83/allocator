#include <iostream>
#include <cassert>
#include <vector>

template<typename T, size_t max>
class MyAllocator {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	template<typename U>
	struct rebind {
		using other = MyAllocator<U, max>;
	};

	MyAllocator() {
		// std::cerr << "MyAllocator():"  << std::endl;
	}

	~MyAllocator() {
		// std::cerr << "~MyAllocator():" << std::endl;
		int i = 0;
		for (auto block : blocks) {
			if (i % max == 0) { // указатели элементов, индекс которых кратен max, указывают на выделяемую област
				auto [ptr, is_busy] = block;
				void* p = static_cast<void*>(ptr);
				std::free(p);
			}
			i++;
		}
	}

	// template<typename U> 
	// MyAllocator(const MyAllocator<U, max>&) {
	// 	std::cout << "MyAllocator (copy)" << std::endl;
	// }

	pointer get_free_block() {
		for (auto& block : blocks) {
			auto& [ptr, is_busy] = block;
			if (!is_busy) {
				is_busy = true;
				return ptr;
			}
		}

		return nullptr;
	}

	pointer allocate(std::size_t n) {
		// std::cerr << "allocate():" << n << std::endl;

		if (n > 1) {
			throw std::invalid_argument("Error: Allocate only one element at a time");
		}

		// поиск свободного блока
		pointer p = get_free_block();
		if (p != nullptr) {
			return p;
		}

		// выделение новой пачки блоков
		void* ptr = std::malloc(max * sizeof(T));
		if (!ptr) {
			throw std::bad_alloc();
		}
		p = reinterpret_cast<pointer>(ptr);
		for (size_t i = 0; i < max; ++i) {
			blocks.push_back(std::make_tuple(p, false));
			p++;
		}

		// получение свободного блока
		p = get_free_block();
		assert (p != nullptr);
		return p;
	}

	void deallocate(pointer p, std::size_t n) {
		// std::cerr << "deallocate():" << n << std::endl;
		if (n > 1) {
			throw std::invalid_argument("Error: Allocate only one element at a time");
		}

		for (auto& block : blocks) {
			auto [ptr, is_busy] = block;
			if (ptr == p && is_busy) {
				is_busy = false;
				break;
			}
		}
	}

	template<typename U, typename ...Args>
	void construct(U* p, Args&& ...args) {
		// std::cerr << "construct():" << std::endl;
		new(p) U(std::forward<Args>(args)...);
	}

	void destroy(pointer p) {
		// std::cerr << "destroy():" << std::endl;
		p->~T();
	}
private:
	// ! теряем stateless для аллокаторов
	std::vector<std::tuple<pointer, bool>>	blocks;	// массив блоков с указателем и признаком свободности
};
