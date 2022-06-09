#include <iostream>
#include <cassert>
#include <vector>
#include <map>
#include "my_allocator.h"
#include "my_list.h"

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