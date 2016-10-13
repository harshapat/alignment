// aligned_storage example
#include <iostream>
#include <type_traits>

struct A {  // non-POD type
	int avg;
	A(int a, int b) : avg((a + b) / 2) {}
};

typedef std::aligned_storage<sizeof(A), alignof(A)>::type A_pod;

int aligned_storage_test() {
	A_pod a, b;
	new (&a) A(10, 20);
	b = a;
	std::cout << reinterpret_cast<A&>(b).avg << std::endl;

	return 0;
}