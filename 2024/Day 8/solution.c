#include "../lib/cutils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// first antenna is at index 0, second at index 1, etc.
struct AntennaMap {
	int *antenna_indices;
	size_t antenna_count;
};

// grups all antennas with the same frequency
struct FrequenciesMap {
	struct AntennaMap frequencies[256];
};

void parse_antennas(char *data, size_t columns, size_t rows,
					struct FrequenciesMap *map) {
	for (size_t i = 0; i < (columns * rows); i++) {
		if (data[i] != '.' && data[i] != '#') {
			size_t antenna_count =
				map->frequencies[(unsigned char)data[i]].antenna_count;
			if (antenna_count == 0) {
				map->frequencies[(unsigned char)data[i]].antenna_indices =
					malloc(sizeof(int) * (columns * rows));
			}
			map->frequencies[(unsigned char)data[i]]
				.antenna_indices[antenna_count] = i;
			map->frequencies[(unsigned char)data[i]].antenna_count++;
		}
	}
}

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

llu part1() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	size_t columns;
	size_t rows;
	data = remove_newlines_and_null_terminator(data, &fsize, &columns, &rows);

	struct FrequenciesMap fmap = {0};
	parse_antennas(data, columns, rows, &fmap);

	bool antinodes[fsize];
	memset(antinodes, 0, fsize * sizeof(bool));

	for (int frequency = 0; frequency < 256; frequency++) {
		struct AntennaMap amap = fmap.frequencies[frequency];
		for (size_t a1_i = 0; a1_i < amap.antenna_count; a1_i++) {
			for (size_t a2_i = a1_i + 1; a2_i < amap.antenna_count; a2_i++) {
				size_t a1 = amap.antenna_indices[a1_i];
				size_t a2 = amap.antenna_indices[a2_i];

				size_t a1_y = a1 / columns;
				size_t a1_x = a1 % columns;

				size_t a2_y = a2 / columns;
				size_t a2_x = a2 % columns;

				int dx = a2_x - a1_x;
				int dy = a2_y - a1_y;

				calc_antinodes(2, a1_x, a1_y, a2_x, a2_y, dx, dy, columns, rows,
							   antinodes);
			}
		}
	}
	llu antinode_count = 0;
	for (size_t i = 0; i < fsize; i++) {
		antinode_count += antinodes[i];
	}

	free(data);
	for (int frequency = 0; frequency < 256; frequency++) {
		free(fmap.frequencies[frequency].antenna_indices);
	}
	return antinode_count;
}

llu part2() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	size_t columns;
	size_t rows;
	data = remove_newlines_and_null_terminator(data, &fsize, &columns, &rows);

	struct FrequenciesMap fmap = {0};
	parse_antennas(data, columns, rows, &fmap);

	bool *antinodes = malloc(sizeof(bool) * fsize);
	for (size_t i = 0; i < fsize; i++) {
		antinodes[i] = false;
	}

	for (int frequency = 0; frequency < 256; frequency++) {
		struct AntennaMap amap = fmap.frequencies[frequency];
		for (size_t a1_i = 0; a1_i < amap.antenna_count; a1_i++) {
			for (size_t a2_i = a1_i + 1; a2_i < amap.antenna_count; a2_i++) {
				size_t a1 = amap.antenna_indices[a1_i];
				size_t a2 = amap.antenna_indices[a2_i];

				size_t a1_y = a1 / columns;
				size_t a1_x = a1 % columns;

				size_t a2_y = a2 / columns;
				size_t a2_x = a2 % columns;

				int dx = a2_x - a1_x;
				int dy = a2_y - a1_y;

				int order = 0;
				bool next_exists = true;
				while (next_exists) {
					next_exists =
						calc_antinodes(order, a1_x, a1_y, a2_x, a2_y, dx, dy,
									   columns, rows, antinodes);
					order++;
				}
			}
		}
	}
	llu antinode_count = 0;
	for (size_t i = 0; i < fsize; i++) {
		antinode_count += antinodes[i];
	}

	free(antinodes);
	free(data);
	for (int frequency = 0; frequency < 256; frequency++) {
		free(fmap.frequencies[frequency].antenna_indices);
	}
	return antinode_count;
}
