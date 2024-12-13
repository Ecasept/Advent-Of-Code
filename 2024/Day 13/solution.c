#include "../lib/cutils.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline llu min(llu a, llu b) { return a < b ? b : a; }

llu parse_num(char **data, char delim) {
	llu num = 0;
	while (**data != delim) {
		num = num * 10 + (**data - '0');
		(*data)++;
	}
	(*data)++;
	return num;
}

llu solve(llu ax, llu ay, llu bx, llu by, llu machine_x, llu machine_y) {
	llu acount = 1;
	llu min_price = ULLONG_MAX;

	llu current_x = 0;
	llu current_y = 0;

	llu max_acount = min(machine_x / ax, machine_y / ay);

	while (acount <= max_acount) {
		current_x += ax;
		current_y += ay;

		llu rest_x = machine_x - current_x;
		llu rest_y = machine_y - current_y;

		if (rest_x % bx == 0 && rest_y % by == 0) {
			if (rest_x / bx == rest_y / by) {
				llu bcount = rest_x / bx;
				llu price = acount * 3 + bcount;
				if (min_price > price) {
					min_price = price;
				}
			}
		}

		acount++;
	}
	if (min_price == ULLONG_MAX) {
		min_price = 0;
	}
	// printf("Price: %llu\n", min_price);
	return min_price;
}

llu parse_machine(char *data) {
	while (*data != '+') {
		data++;
	}
	data++;
	llu ax = parse_num(&data, ',');
	data += 3;
	llu ay = parse_num(&data, '\n');

	while (*data != '+') {
		data++;
	}
	data++;
	llu bx = parse_num(&data, ',');
	data += 3;
	llu by = parse_num(&data, '\n');

	while (*data != '=') {
		data++;
	}
	data++;
	llu machine_x = parse_num(&data, ',');
	data += 3;
	llu machine_y = parse_num(&data, '\0');
	return solve(ax, ay, bx, by, machine_x, machine_y);
}

llu part1() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);

	char *token = data;
	char *next;

	llu sum = 0;
	while ((next = strstr(token, "\n\n")) != NULL) {
		*next = '\0';
		sum += parse_machine(token);
		token = next + 2;
	}

	// final line
	next = token;
	while ((*next) != '\0') {
		next++;
	}
	*(next - 1) = '\0';
	sum += parse_machine(token);

	free(data);
	return sum;
}

llu part2() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);

	free(data);
	return 0;
}
