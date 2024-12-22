#include "../lib/cutils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int WIDTH = 101;
const int HEIGHT = 103;

struct Robot {
	int pos_x;
	int pos_y;
	int vel_x;
	int vel_y;
};

typedef struct Robot Robot;

// in c the result of a modulo operation has the sign of the dividend
// this is a python-like implementation where it wraps around to positive
unsigned modulo(int value, unsigned m) {
	int mod = value % (int)m;
	if (mod < 0) {
		mod += m;
	}
	return mod;
}

int to_quadrant(int x, int y, llu space_x, llu space_y) {
	int middle_x = space_x / 2;
	int middle_y = space_y / 2;
	if (x < middle_x && y < middle_y) {
		return 0; // first quadrant
	} else if (x > middle_x && y < middle_y) {
		return 1; // second quadrant
	} else if (x < middle_x && y > middle_y) {
		return 2; // third quadrant
	} else if (x > middle_x && y > middle_y) {
		return 3; // fourth quadrant
	}
	return -1; // middle row/column
}

void simulate(int *x, int *y, Robot *r, int seconds, int space_x, int space_y) {
	*x = modulo(r->pos_x + r->vel_x * (int)seconds, space_x);
	*y = modulo(r->pos_y + r->vel_y * (int)seconds, space_y);
}

Robot *parse_input(char *file, int *robot_count) {
	const int MAX_ROBOT_COUNT = 600; // 500 for my input
	Robot *robots = malloc(sizeof(Robot) * MAX_ROBOT_COUNT);
	*robot_count = 0;

	size_t fsize;
	char *data = load_file(file, &fsize);

	char *line = strtok(data, "\n");
	while (line != NULL) {
		if (*robot_count >= MAX_ROBOT_COUNT) {
			printf("To many robots\n");
			exit(1);
		}
		Robot *r = &robots[*robot_count]; // current robot
		sscanf(line, "p=%d,%d v=%d,%d", &r->pos_x, &r->pos_y, &r->vel_x,
			   &r->vel_y);
		line = strtok(NULL, "\n");
		(*robot_count)++;
	}

	free(data);
	return robots;
}

void print_robots(Robot *robots, int robot_count, int space_x, int space_y) {
	// print robots into a grid
	for (int y = 0; y < space_y; y++) {
		for (int x = 0; x < space_x; x++) {
			bool found = false;
			for (int i = 0; i < robot_count; i++) {
				if (robots[i].pos_x == x && robots[i].pos_y == y) {
					// mark middle
					printf("#");
					found = true;
					break;
				}
			}
			if (!found) {
				printf(".");
			}
		}
		printf("\n");
	}
	printf("\n\n\n");
}

llu part1(void) {
	int robot_count;
	Robot *robots = parse_input("input.txt", &robot_count);
	int quadrants[4] = {0};
	for (int i = 0; i < robot_count; i++) {
		Robot *r = &robots[i];

		simulate(&r->pos_x, &r->pos_y, r, 100, WIDTH, HEIGHT);

		int res = to_quadrant(r->pos_x, r->pos_y, WIDTH, HEIGHT);
		if (res >= 0) {
			quadrants[res]++;
		}
	}

	// print_robots(robots, robot_count, WIDTH, HEIGHT);

	free(robots);
	return quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
}

llu part2(void) {
	int robot_count;
	Robot *robots = parse_input("input.txt", &robot_count);

	int horizontal_sum = 0;
	int avg = 0;

	// grid to store whether a robot is at a position
	bool robot_grid[WIDTH][HEIGHT];
	memset(robot_grid, false, sizeof(robot_grid));

	// simulate steps
	// pattern repeats after 10403 steps
	for (int steps = 1; steps < 10403; steps++) {

		// update robot positions
		for (int i = 0; i < robot_count; i++) {
			Robot *r = &robots[i];
			robot_grid[r->pos_x][r->pos_y] = false;

			simulate(&r->pos_x, &r->pos_y, r, 1, WIDTH, HEIGHT);
		}

		// move positions to grid
		for (int i = 0; i < robot_count; i++) {
			robot_grid[robots[i].pos_x][robots[i].pos_y] = true;
		}

		// check how many horizontal lines of robots there are
		int horizontal_count = 0;
		for (int i = 0; i < HEIGHT; i++) {
			bool last_was_robot = false;
			for (int j = 0; j < WIDTH; j++) {
				if (robot_grid[j][i]) {
					if (last_was_robot) {
						horizontal_count++;
					}
					last_was_robot = true;
				} else {
					last_was_robot = false;
				}
			}
		}

		if (steps < 10) {
			// update average (only for the first 10 steps)
			horizontal_sum += horizontal_count;
			avg = horizontal_sum / (steps + 1);
		} else if (avg * 8 < horizontal_count) {
			// if there are more than 8 times the average amount of robots in a
			// row

			// print_robots(robots, robot_count, WIDTH, HEIGHT);
			free(robots);
			return steps;
		}
	}
	printf("Could not find tree\n");
	exit(1);
}
