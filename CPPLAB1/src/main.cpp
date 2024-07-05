#include <iostream>
#include "BucketStorage.hpp"

int main()
{
	BucketStorage<int> b;
	std::cout << "Hello, World!" << std::endl;
	b.insert(10);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
