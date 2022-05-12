#include <iostream>
#include <cassert>
#include <vector>
#include <map>

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
	};

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
	};

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

long factorial(const int n)
{
	if (n == 0 || n == 1) {
		return n;
	}
	long f = 1;
	for (int i = 1; i <= n; ++i) {
		f *= i;
	}
	return f;
}

template <typename T, typename Allocator = std::allocator<T>>
class MyList
{
public:
	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using const_reference = const T&;

	struct Item;
	using item_type = Item;
	using item_pointer = Item*;
	using allocator_type = typename Allocator::template rebind<Item>::other;

	struct Item {
		T					_value;
		item_pointer		_next;
		item_pointer		_prev;

		Item(const_reference value, item_pointer next, item_pointer prev): _value{value}, _next{next}, _prev{prev} {};
	};

	MyList() {};
	~MyList() {
		item_pointer	p = first;
		while (p) {
			item_pointer	p_next = p->_next;
			allocator.destroy(p);
			allocator.deallocate(p, 1);
			p = p_next;
		}

	};
	void push_back(const_reference value) {
		item_pointer p = allocator.allocate(1);
		if (first == nullptr) {
			allocator.construct(p, value, first, last);
			first = last = p;
		}
		else {
			last->_next = p;
			allocator.construct(p, value, nullptr, last);
			last = p;
		}
	};

	reference	operator[](int index) {
		item_pointer	p = first;
		for (int i = 0; p; i++) {
			if (i == index) {
				return p->_value;
			}
			p = p->_next;
		}
		throw std::range_error("Error: Invalid index");
	}

private:
	allocator_type	allocator;
	item_pointer	first{nullptr};
	item_pointer	last{nullptr};
};

int main(int, char**)
{
	try {
		std::vector<int> range = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
		std::map<int, int> m1;
		for (auto& i : range) {
			m1[i] = factorial(i);
		}

		// MyAllocator
		std::map<int, int, std::less<int>, MyAllocator<std::pair<const int, int>, 10>> m2;
		for (auto& i : range) {
			long l = factorial(i);
			m2[i] = l;
		}

		for (auto& i : range) {
			std::cout << i << " " << m2[i] << std::endl;;
		}

		// MyList
		MyList<int> l1;
		for (auto& i : range) {
			long l = factorial(i);
			l1.push_back(l);
		}

		for (auto& i : range) {
			std::cout << i << " " << l1[i] << std::endl;;
		}

		// MyList + MyAllocator
		MyList<int, MyAllocator<int, 10>> l2;
		for (auto& i : range) {
			long l = factorial(i);
			l2.push_back(l);
		}

		for (auto& i : range) {
			std::cout << i << " " << l2[i] << std::endl;;
		}
	}
	catch(const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}