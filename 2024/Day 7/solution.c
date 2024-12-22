#include "../lib/cutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

llu parse(llu test_value, llu cur_val, char *remaining_chars) {
	if (cur_val != -1 && cur_val > test_value) {
		return 0;
	}
	char *num_start =
		remaining_chars; // copy because strtoken_r changes remaining_chars
	char *num_str = strtok_r(remaining_chars, " ", &remaining_chars);
	if (num_str == NULL) {
		if (test_value == cur_val) {
			return test_value;
		}
		return 0;
	}
	// strtok_r sets the delimiter before the current token to \0, but that
	// breaks the recursiveness of this function
	num_start[-1] = ' ';

	llu ret;
	llu num = strtoull(num_str, NULL, 10);

	// first number does not need to be added or multiplied
	if (cur_val == -1) {
		return parse(test_value, num, remaining_chars);
	}

	ret = parse(test_value, cur_val * num, remaining_chars);
	if (ret > 0) {
		// return early (already found match)
		return ret;
	}

	ret = parse(test_value, cur_val + num, remaining_chars);
	return ret;
}

llu parse_line(char *line) {
	char *num_str;
	char *remaining_chars = line;

	num_str = strtok_r(line, ":", &remaining_chars);
	llu test_value = strtoull(num_str, NULL, 10);
	return parse(test_value, -1, remaining_chars);
}

llu part1(void) {
	size_t fsize; // does not include null terminator
	char *data = load_file("input.txt", &fsize);

	char *line;
	llu sum = 0;

	line = strtok(data, "\n");
	while (line != NULL) {

		llu ret = parse_line(line);
		sum += ret;

		line = strtok(NULL, "\n");
	}

	free(data);
	return sum;
}

llu concatenate(llu x, llu y) {
	llu pow = 10;
	while (y >= pow)
		pow *= 10;
	return x * pow + y;
}

llu parse2(llu test_value, llu cur_val, char *remaining_chars) {
	// if num is already bigger than test_value, this branch will never succeed
	if (cur_val != -1 && cur_val > test_value) {
		return 0;
	}
	// base case
	// return if end of string reached
	if (*remaining_chars == '\0') {
		if (test_value == cur_val) {
			return test_value;
		}
		return 0;
	}

	remaining_chars++; // skip delimiter

	// find end of string
	char *end_ptr = remaining_chars;
	llu num = 0;
	while (*end_ptr != ' ' && *end_ptr != '\0') {
		num = num * 10 + *end_ptr - '0';
		end_ptr++;
	}

	// first number does not need to be added or multiplied
	if (cur_val == -1) {
		return parse2(test_value, num, end_ptr);
	}

	llu ret;
	ret = parse2(test_value, cur_val + num, end_ptr);
	if (ret > 0) {
		return ret;
	}

	ret = parse2(test_value, cur_val * num, end_ptr);
	if (ret > 0) {
		return ret;
	}

	ret = parse2(test_value, concatenate(cur_val, num), end_ptr);
	return ret;
}

llu parse_line2(char *line) {
	char *num_str;
	char *remaining_chars = line;

	num_str = strtok_r(line, ":", &remaining_chars);
	llu test_value = strtoull(num_str, NULL, 10);
	return parse2(test_value, -1, remaining_chars);
}

llu part2(void) {
	size_t fsize; // does not include null terminator
	char *data = load_file("input.txt", &fsize);

	char *line;
	llu sum = 0;

	line = strtok(data, "\n");
	while (line != NULL) {

		llu ret = parse_line2(line);
		sum += ret;

		line = strtok(NULL, "\n");
	}

	free(data);
	return sum;
}
