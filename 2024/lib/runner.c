#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned long long llu;

llu part1();
llu part2();

void benchmark_function(llu (*func)(), const char *name) {
	printf("%s: ", name);

	// Start time
	clock_t start = clock();

	// Call the function
	llu res = func();

	// End time
	clock_t end = clock();

	// Calculate elapsed time in seconds
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;

	printf("\033[33m%f seconds\033[0m\n", elapsed);
	printf("Result: %llu\n", res);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("%llu\n", part1());
		printf("%llu\n", part2());
	} else if (strcmp(argv[1], "1") == 0) {
		if (argc >= 3 && strcmp(argv[2], "benchmark") == 0) {
			benchmark_function(part1, "Part 1");
		} else {
			printf("%llu\n", part1());
		}
	} else if (strcmp(argv[1], "2") == 0) {
		if (argc >= 3 && strcmp(argv[2], "benchmark") == 0) {
			benchmark_function(part2, "Part 2");
		} else {
			printf("%llu\n", part2());
		}
	} else {
		printf("Usage: %s [1|2]\n", argv[0]);
		return 1;
	}
	return 0;
}
