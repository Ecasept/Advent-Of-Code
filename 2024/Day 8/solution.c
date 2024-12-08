#include "../lib/cutils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// check if nth order antinodes exist, if yes then set them and return true,
// otherwise return false
bool calc_antinodes(int order, int a1_x, int a1_y, int a2_x, int a2_y, int dx,
					int dy, int columns, int rows, bool *antinodes) {
	bool did_exist = false;
	int new_x1 = a1_x + order * dx;
	int new_y1 = a1_y + order * dy;
	int new_x2 = a2_x - order * dx;
	int new_y2 = a2_y - order * dy;
	if (new_x1 >= 0 && new_y1 >= 0 && new_x1 < columns && new_y1 < rows) {
		int antinode1_index = new_y1 * columns + new_x1;
		antinodes[antinode1_index] = true;
		did_exist = true;
	}
	if (new_x2 >= 0 && new_y2 >= 0 && new_x2 < columns && new_y2 < rows) {
		int antinode2_index = new_y2 * columns + new_x2;
		antinodes[antinode2_index] = true;
		did_exist = true;
	}
	return did_exist;
}

size_t get_antenna_count(char *data, size_t columns, size_t rows) {
	size_t count = 0;
	for (size_t i = 0; i < columns; i++) {
		for (size_t j = 0; j < rows; j++) {
			if (data[i * rows + j] != '.' && data[i * rows + j] != '#') {
				count++;
			}
		}
	}
	return count;
}

size_t *get_antennas(char *data, size_t columns, size_t rows, size_t count,
					 size_t *length) {
	size_t *antennas = malloc(count * sizeof(size_t));
	*length = 0;
	for (size_t i = 0; i < columns; i++) {
		for (size_t j = 0; j < rows; j++) {
			size_t index = i * rows + j;
			if (data[index] != '.' && data[i * rows + j] != '#') {
				antennas[*length] = index;
				(*length)++;
			}
		}
	}
	if (count != *length) {
		printf("Error: count != length\n");
		exit(1);
	}
	return antennas;
}

llu part1() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	size_t columns;
	size_t rows;
	data = remove_newlines_and_null_terminator(data, &fsize, &columns, &rows);

	size_t count = get_antenna_count(data, columns, rows);
	size_t antenna_count;
	size_t *antennas = get_antennas(data, columns, rows, count, &antenna_count);

	bool *antinodes = malloc(sizeof(bool) * fsize);
	for (size_t i = 0; i < fsize; i++) {
		antinodes[i] = false;
	}

	for (size_t a1_i = 0; a1_i < antenna_count; a1_i++) {
		for (size_t a2_i = a1_i + 1; a2_i < antenna_count; a2_i++) {
			size_t a1 = antennas[a1_i];
			size_t a2 = antennas[a2_i];
			if (data[a1] != data[a2]) {
				// antennas don't have the same frequence
				continue;
			}
			size_t a1_y = a1 / columns;
			size_t a1_x = a1 % columns;

			size_t a2_y = a2 / columns;
			size_t a2_x = a2 % columns;

			int dx = a2_x - a1_x;
			int dy = a2_y - a1_y;

			// first order antinodes (where antenna is twice as far away)
			calc_antinodes(2, a1_x, a1_y, a2_x, a2_y, dx, dy, columns, rows,
						   antinodes);
		}
	}
	llu antinode_count = 0;
	for (size_t i = 0; i < fsize; i++) {
		antinode_count += antinodes[i];
	}

	free(antinodes);
	free(antennas);
	free(data);
	return antinode_count;
}

llu part2() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	size_t columns;
	size_t rows;
	data = remove_newlines_and_null_terminator(data, &fsize, &columns, &rows);

	size_t count = get_antenna_count(data, columns, rows);
	size_t antenna_count;
	size_t *antennas = get_antennas(data, columns, rows, count, &antenna_count);

	bool *antinodes = malloc(sizeof(bool) * fsize);
	for (size_t i = 0; i < fsize; i++) {
		antinodes[i] = false;
	}

	for (size_t a1_i = 0; a1_i < antenna_count; a1_i++) {
		for (size_t a2_i = a1_i + 1; a2_i < antenna_count; a2_i++) {
			size_t a1 = antennas[a1_i];
			size_t a2 = antennas[a2_i];
			if (data[a1] != data[a2]) {
				// antennas don't have the same frequence
				continue;
			}
			size_t a1_y = a1 / columns;
			size_t a1_x = a1 % columns;

			size_t a2_y = a2 / columns;
			size_t a2_x = a2 % columns;

			int dx = a2_x - a1_x;
			int dy = a2_y - a1_y;

			int order = 0;
			bool next_exists = true;
			while (next_exists) {
				next_exists = calc_antinodes(order, a1_x, a1_y, a2_x, a2_y, dx,
											 dy, columns, rows, antinodes);
				order++;
			}
		}
	}
	llu antinode_count = 0;
	for (size_t i = 0; i < fsize; i++) {
		antinode_count += antinodes[i];
	}

	free(antinodes);
	free(antennas);
	free(data);
	return antinode_count;
}
