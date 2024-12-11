#include "../lib/cutils.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_num_length(llu num) {
	if (num >= 1000000000)
		return 10;
	if (num >= 100000000)
		return 9;
	if (num >= 10000000)
		return 8;
	if (num >= 1000000)
		return 7;
	if (num >= 100000)
		return 6;
	if (num >= 10000)
		return 5;
	if (num >= 1000)
		return 4;
	if (num >= 100)
		return 3;
	if (num >= 10)
		return 2;
	if (num >= 1)
		return 1;
	printf("to large number\n");
	exit(1);
}

// https://stackoverflow.com/a/32016977
void split_num(llu num, llu *first_half, llu *second_half) {
	const unsigned int BASE = 10;
	unsigned int divisor = BASE;

	while (num / divisor > divisor) {
		divisor *= BASE;
	}

	*first_half = num / divisor;
	*second_half = num % divisor;
}

llu cache_size;
llu max_level;
llu **cache;

llu count_number(llu num, unsigned level) {
	// NOTE: level is zero indexed, max_level not!
	// base case
	if (level >= max_level) {
		return 1;
	}

	// check cache
	if (num < cache_size) {
		if (cache[num][level] != ULLONG_MAX) {
			return cache[num][level];
		}
	}

	llu res;
	if (num == 0) {
		res = count_number(1, level + 1);
	} else if (get_num_length(num) % 2 == 0) {
		llu first_half;
		llu second_half;
		split_num(num, &first_half, &second_half);
		res = count_number(first_half, level + 1) +
			  count_number(second_half, level + 1);
	} else {
		res = count_number(num * 2024, level + 1);
	}
	if (num < cache_size) {
		cache[num][level] = res;
	}
	return res;
}

void create_cache() {
	cache = (llu **)malloc(cache_size * sizeof(llu *));
	for (unsigned num = 0; num < cache_size; num++) {
		cache[num] = (llu *)malloc(max_level * sizeof(llu));
		for (unsigned level = 0; level < max_level; level++) {
			cache[num][level] = ULLONG_MAX;
		}
	}
}

void free_cache() {
	for (llu i = 0; i < cache_size; i++) {
		free(cache[i]);
	}
	free(cache);
}

llu solve() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);

	create_cache();

	char *num_str;
	llu sum = 0;

	num_str = strtok(data, " ");
	while (num_str != NULL) {

		llu num = strtoull(num_str, NULL, 10);

		sum += count_number(num, 0);

		num_str = strtok(NULL, " ");
	}

	free_cache();
	free(data);
	return sum;
}

llu part1() {
	cache_size = 1000;
	max_level = 25;
	return solve();
}

llu part2() {
	cache_size = 1000;
	max_level = 75;
	return solve();
}
