#include "../cutils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

typedef unsigned long long big;

big parse(big test_value, big cur_val, char **remaining_chars_ptr) {
	// get number and remaining characters

	char *num_start =
		*remaining_chars_ptr; // copy start of num because remaining_chars_ptr
							  // is changed by strtok_r

	char *num_str = strtok_r(*remaining_chars_ptr, " ", remaining_chars_ptr);
	if (num_str == NULL) {
		if (test_value == cur_val) {
			return test_value;
		}
		return 0;
	}
	// strtok_r sets the delimiter before the current token to \0, but that
	// breaks the recursiveness of this function
	num_start[-1] = ' ';

	big ret;
	big num = strtoull(num_str, NULL, 10);

	// first number does not need to be added or multiplied
	if (cur_val == -1) {
		return parse(test_value, num, remaining_chars_ptr);
	}

	char *this_start = *remaining_chars_ptr;
	ret = parse(test_value, cur_val * num, remaining_chars_ptr);
	if (ret > 0) {
		// return early
		return ret;
	}

	ret = parse(test_value, cur_val + num, &this_start);
	return ret;
}

big parse_line(char *line) {
	char *num_str;
	char *remaining_chars = line;

	num_str = strtok_r(line, ":", &remaining_chars);
	big test_value = strtoull(num_str, NULL, 10);
	return parse(test_value, -1, &remaining_chars);
}

void part1() {
	size_t fsize; // does not include null terminator
	char *data = load_file("input.txt", &fsize);

	char *line;
	big sum = 0;

	line = strtok(data, "\n");
	while (line != NULL) {

		big ret = parse_line(line);
		printf("%llu\n", ret);
		sum += ret;

		line = strtok(NULL, "\n");
	}

	printf("sum: %llu\n", sum);
}

void part2() {
	// auto file = utils::getExample(1);
	// std::string line;

	// while (std::getline(file, line)) {
	// }
	// std::cout << "not implemented" << '\n';
	return;
}

int main() {
	part1();
	part2();
	return 0;
}
