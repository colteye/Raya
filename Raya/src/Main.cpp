#include "projects/Project2.h"
#include "projects/Project1.h"

#include <iostream>
#include <thread>

int main(int argc, char **argv) {

	std::cout << "thread count: " << std::thread::hardware_concurrency();

	project1();
	project2();
	return 0;
}