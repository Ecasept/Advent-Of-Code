#include "../lib/cutils.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Minimum amount of turns to get from one direction to another
// Calculates minimum between clockwise and counter-clockwise turns
int min_turns(int from, int to) {
	return ((to - from + 4) % 4 < (from - to + 4) % 4) ? (to - from + 4) % 4
													   : (from - to + 4) % 4;
}

void print_grid(char *grid, size_t width, size_t height, size_t cur_x,
				size_t cur_y, int cur_dir, int cost) {
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			if (i == cur_y && j == cur_x) {
				char dirs[] = {'^', '>', 'v', '<'};
				printf("%c", dirs[cur_dir]);
			} else {
				printf("%c", grid[i * width + j]);
			}
		}
		printf("\n");
	}
	printf("Cost: %d\n", cost);
	printf("\n");
}

void dfs(char *grid, int dir, int cost, size_t x, size_t y, size_t width,
		 size_t height, int *distances) {
	char chr = grid[y * width + x];
	switch (chr) {
	case '#':
		return;
	case 'E':
		if (distances[y * width + x] > cost) {
			distances[y * width + x] = cost;
		}
		return;
	case 'S':
	case '.':
		if (distances[y * width + x] < cost) {
			// already visited with lower cost
			return;
		}
		// print_grid(grid, width, height, x, y, dir, cost);
		distances[y * width + x] = cost;
		for (int next_dir = 0; next_dir < 4; next_dir++) {
			int dx = 0, dy = 0;
			switch (next_dir) {
			case 0:
				dy = -1;
				break;
			case 1:
				dx = 1;
				break;
			case 2:
				dy = 1;
				break;
			case 3:
				dx = -1;
				break;
			}
			//  0
			// 3 1
			//  2
			// amount of 90 deg turns to get from dir to next_dir
			int turns = min_turns(dir, next_dir);
			int next_cost = cost + 1 + turns * 1000;
			dfs(grid, next_dir, next_cost, x + dx, y + dy, width, height,
				distances);
		}
	}
}

void find_chr(char *grid, size_t width, size_t height, size_t *x, size_t *y,
			  char chr) {
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			if (grid[i * width + j] == chr) {
				*x = j;
				*y = i;
				return;
			}
		}
	}
	printf("Could not find %c", chr);
	exit(1);
}

llu part1(void) {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	size_t width, height;
	char *grid =
		remove_newlines_and_null_terminator(data, &fsize, &width, &height);

	size_t start_x, start_y, end_x, end_y;
	find_chr(grid, width, height, &start_x, &start_y, 'S');
	find_chr(grid, width, height, &end_x, &end_y, 'E');

	int distances[width * height];
	for (size_t i = 0; i < width * height; i++) {
		distances[i] = INT_MAX;
	}

	dfs(grid, 1, 0, start_x, start_y, width, height, distances);

	llu sum = distances[end_y * width + end_x];

	free(grid);
	return sum;
}

bool dfs2(char *grid, int dir, int cost, size_t x, size_t y, size_t width,
		  size_t height, int distances[][4], bool *best_paths, int min_cost) {
	char chr = grid[y * width + x];
	switch (chr) {
	case '#':
		return false;
	case 'E':
		if (cost == min_cost) {
			best_paths[y * width + x] = true;
			return true;
		}
		return false;
	case 'S':
	case '.':
		if (distances[y * width + x][dir] < cost) {
			// already visited with lower cost
			return false;
		}
		if (cost > min_cost) {
			// can't be a best path
			return false;
		}
		bool is_a_best_path = false;
		// print_grid(grid, width, height, x, y, dir, cost);
		distances[y * width + x][dir] = cost;
		for (int next_dir = 0; next_dir < 4; next_dir++) {
			int dx = 0, dy = 0;
			switch (next_dir) {
			case 0:
				dy = -1;
				break;
			case 1:
				dx = 1;
				break;
			case 2:
				dy = 1;
				break;
			case 3:
				dx = -1;
				break;
			}
			//  0
			// 3 1
			//  2
			// amount of 90 deg turns to get from dir to next_dir
			int turns = min_turns(dir, next_dir);
			int next_cost = cost + 1 + turns * 1000;
			is_a_best_path =
				is_a_best_path | dfs2(grid, next_dir, next_cost, x + dx, y + dy,
									  width, height, distances, best_paths,
									  min_cost);
		}
		if (is_a_best_path) {
			best_paths[y * width + x] = true;
		}
		return is_a_best_path;
	}
	printf("Invalid character %c", chr);
	exit(1);
}

void print_best_paths_grid(char *grid, bool *best_paths, size_t width,
						   size_t height, size_t cur_x, size_t cur_y,
						   int cur_dir, int cost) {
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			if (i == cur_y && j == cur_x) {
				char dirs[] = {'^', '>', 'v', '<'};
				printf("%c", dirs[cur_dir]);
			} else if (best_paths[i * width + j]) {
				printf("\033[31mO\033[0m");
			} else {
				printf("%c", grid[i * width + j]);
			}
		}
		printf("\n");
	}
	printf("Cost: %d\n", cost);
	printf("\n");
}

llu part2(void) {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	size_t width, height;
	char *grid =
		remove_newlines_and_null_terminator(data, &fsize, &width, &height);

	size_t start_x, start_y, end_x, end_y;
	find_chr(grid, width, height, &start_x, &start_y, 'S');
	find_chr(grid, width, height, &end_x, &end_y, 'E');

	int distances[width * height];
	for (size_t i = 0; i < width * height; i++) {
		distances[i] = INT_MAX;
	}

	dfs(grid, 1, 0, start_x, start_y, width, height, distances);
	llu min_cost = distances[end_y * width + end_x];

	int distances2[width * height][4];
	bool best_paths[width * height];
	for (size_t i = 0; i < width * height; i++) {
		distances2[i][0] = INT_MAX;
		distances2[i][1] = INT_MAX;
		distances2[i][2] = INT_MAX;
		distances2[i][3] = INT_MAX;
		best_paths[i] = false;
	}

	dfs2(grid, 1, 0, start_x, start_y, width, height, distances2, best_paths,
		 min_cost);

	// print_best_paths_grid(grid, best_paths, width, height, end_x, end_y, 1,
	// 					  distances2[end_y * width + end_x][0]);

	int best_paths_count = 0;
	for (size_t i = 0; i < width * height; i++) {
		if (best_paths[i]) {
			best_paths_count++;
		}
	}

	free(grid);
	return best_paths_count;
}
