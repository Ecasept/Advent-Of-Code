#include <chrono>
#include <iostream>
#include <string.h>

typedef unsigned long long llu;

llu part1();
llu part2();

void benchmark_function(llu (*func)(), const char *name) {
	std::cout << name << ": ";
	auto start = std::chrono::high_resolution_clock::now();
	llu res = func();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "\033[33m" << elapsed.count() << " seconds\033[0m\n";
	std::cout << "Result: " << res << std::endl;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		std::cout << part1() << "\n";
		std::cout << part2() << "\n";
	} else if (strcmp(argv[1], "1") == 0) {
		if (argc >= 3 && strcmp(argv[2], "benchmark") == 0) {
			benchmark_function(part1, "Part 1");
		} else {
			std::cout << part1() << "\n";
		}
	} else if (strcmp(argv[1], "2") == 0) {
		if (argc >= 3 && strcmp(argv[2], "benchmark") == 0) {
			benchmark_function(part2, "Part 2");
		} else {
			std::cout << part2() << "\n";
		}
	} else if (strcmp(argv[1], "benchmark") == 0) {
		benchmark_function(part1, "Part 1");
		benchmark_function(part2, "Part 2");
	} else {
		std::cout << "Usage: " << argv[0] << " [1|2] [benchmark]\n";
		return 1;
	}
	return 0;
}
