#include "../lib/cutils.h"
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

llu parse_num(char **data, char delim) {
	llu num = 0;
	while (**data != delim) {
		num = num * 10 + (**data - '0');
		(*data)++;
	}
	(*data)++;
	return num;
}

// See math.py for explanation
double solvey(double ax, double ay, double bx, double by, double cx,
			  double cy) {
	double numerator = cy - (ay * cx / ax);
	double denominator = -(ay * bx / ax) + by;
	return numerator / denominator;
}

double solvex(double cx, double bx, double ax, double y) {
	return (cx - bx * y) / ax;
}

// tolerance against floating point errors
const double FLOATING_POINT_EPSILON = 1e-3;

bool is_integer(double num) {
	return fabs(num - round(num)) < FLOATING_POINT_EPSILON;
}

long long to_integer(double num) {
	return (long long)(num + FLOATING_POINT_EPSILON);
}

llu solve(llu ax, llu ay, llu bx, llu by, llu machine_x, llu machine_y) {

	double y = solvey(ax, ay, bx, by, machine_x, machine_y);
	double x = solvex(machine_x, bx, ax, y);

	if (!is_integer(x)) {
		return 0;
	}
	if (!is_integer(y)) {
		return 0;
	}
	return to_integer(x) * 3 + to_integer(y);
}

llu parse_machine(char *data, llu increase) {
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
	return solve(ax, ay, bx, by, machine_x + increase, machine_y + increase);
}

llu solve_puzzle(llu increase) {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);

	char *token = data;
	char *next;

	llu sum = 0;
	while ((next = strstr(token, "\n\n")) != NULL) {
		*next = '\0';
		sum += parse_machine(token, increase);
		token = next + 2;
	}

	// final line
	next = token;
	while ((*next) != '\0') {
		next++;
	}
	*(next - 1) = '\0';
	sum += parse_machine(token, increase);

	free(data);
	return sum;
}

llu part1() { return solve_puzzle(0); }

llu part2() { return solve_puzzle(10000000000000); }
