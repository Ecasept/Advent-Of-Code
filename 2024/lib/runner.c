#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
typedef unsigned long long llu;

// Macro to define return type based on preprocessor flags
#ifdef STRING_RESULT_PART1
typedef char *return_type_part1;
#else
typedef llu return_type_part1;
#endif

#ifdef STRING_RESULT_PART2
typedef char *return_type_part2;
#else
typedef llu return_type_part2;
#endif

return_type_part1 part1(void);
return_type_part2 part2(void);

void benchmark_function_part1(return_type_part1 (*func)(void)) {
	printf("Part 1: ");
	// Start time
	clock_t start = clock();
	// Call the function
	return_type_part1 res = func();
	// End time
	clock_t end = clock();
	// Calculate elapsed time in seconds
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("\033[33m%f seconds\033[0m\n", elapsed);
#ifdef STRING_RESULT_PART1
	printf("Result: %s\n", res);
	free(res);
#else
	printf("Result: %llu\n", res);
#endif
}

void benchmark_function_part2(return_type_part2 (*func)(void)) {
	printf("Part 2: ");
	// Start time
	clock_t start = clock();
	// Call the function
	return_type_part2 res = func();
	// End time
	clock_t end = clock();
	// Calculate elapsed time in seconds
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("\033[33m%f seconds\033[0m\n", elapsed);
#ifdef STRING_RESULT_PART2
	printf("Result: %s\n", res);
	free(res);
#else
	printf("Result: %llu\n", res);
#endif
}

void print_res_part1(return_type_part1 res) {
#ifdef STRING_RESULT_PART1
	printf("%s\n", res);
	free(res);
#else
	printf("%llu\n", res);
#endif
}

void print_res_part2(return_type_part2 res) {
#ifdef STRING_RESULT_PART2
	printf("%s\n", res);
	free(res);
#else
	printf("%llu\n", res);
#endif
}

int main(int argc, char **argv) {
	if (argc < 2) {
		print_res_part1(part1());
		print_res_part2(part2());
	} else if (strcmp(argv[1], "1") == 0) {
		if (argc >= 3 && strcmp(argv[2], "benchmark") == 0) {
			benchmark_function_part1(part1);
		} else {
			print_res_part1(part1());
		}
	} else if (strcmp(argv[1], "2") == 0) {
		if (argc >= 3 && strcmp(argv[2], "benchmark") == 0) {
			benchmark_function_part2(part2);
		} else {
			print_res_part2(part2());
		}
	} else if (strcmp(argv[1], "benchmark") == 0) {
		benchmark_function_part1(part1);
		benchmark_function_part2(part2);
	} else {
		printf("Usage: %s [1|2] [benchmark]\n", argv[0]);
		return 1;
	}
	return 0;
}
