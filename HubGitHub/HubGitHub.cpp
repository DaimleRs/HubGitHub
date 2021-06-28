#include <iostream>
#include <algorithm>

#include "DinamicArray.hpp"






int main()
{
	DinamicArray<int> q;
	for (size_t i = 0; i < 20; i++) {
		q.pushBack(i + 1);
	}
	for (auto e : q) {
		std::cout << e << "\n";
	}
	return 0;