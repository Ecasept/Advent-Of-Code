#include "../lib/cutils.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

int quick_pow10(int n) {
	static int pow10[10] = {1,		10,		 100,	   1000,	  10000,
							100000, 1000000, 10000000, 100000000, 1000000000};

	return pow10[n];
}

int get_num_length(llu num) {
	int length = 1;
	while (num >= 10) {
		num /= 10;
		length++;
	}
	return length;
}

void split_num(llu num, int split_size, llu *first_half, llu *second_half) {
	int moved_count = 0;

	*first_half = num;
	*second_half = 0;
	while (moved_count < split_size) {
		*first_half /= 10;
		moved_count++;
	}

	*second_half = num % ((*first_half) * quick_pow10(moved_count));
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
		if (cache[num][level] != -1) {
			return cache[num][level];
		}
	}

	llu res;
	if (num == 0) {
		res = count_number(1, level + 1);
	} else {
		int num_length = get_num_length(num);
		if (num_length % 2 == 0) {
			llu first_half;
			llu second_half;
			split_num(num, num_length / 2, &first_half, &second_half);
			res = count_number(first_half, level + 1) +
				  count_number(second_half, level + 1);
		} else {
			res = count_number(num * 2024, level + 1);
		}
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
			cache[num][level] = -1;
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
	cache_size = 100000;
	max_level = 25;
	return solve();
}

llu part2() {
	cache_size = 100000;
	max_level = 75;
	return solve();
}
