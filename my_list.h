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
