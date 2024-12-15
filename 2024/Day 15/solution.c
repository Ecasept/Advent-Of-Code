#include "../lib/cutils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Robot {
	int x;
	int y;
};

typedef struct Robot Robot;

void print_data(char *data, size_t width, size_t height, Robot r) {
	for (size_t i = 0; i < width * height; i++) {
		if (r.x == (int)i % (int)width && r.y == (int)i / (int)width) {
			printf("@");
		} else {
			printf("%c", data[i]);
		}
		if ((i + 1) % width == 0) {
			printf("\n");
		}
	}
	printf("\n\n");
}

void get_robot_pos(char *data, size_t width, size_t height, Robot *r) {
	for (size_t i = 0; i < width * height; i++) {
		if (data[i] == '@') {
			r->x = i % width;
			r->y = i / width;
			return;
		}
	}
	printf("Robot not found\n");
	exit(1);
}

void get_movement_vector(char instruction, int movement_vector[2]) {
	int vectors[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
	char instructions[4] = {'^', '>', 'v', '<'};
	for (int i = 0; i < 4; i++) {
		if (instruction == instructions[i]) {
			movement_vector[0] = vectors[i][0];
			movement_vector[1] = vectors[i][1];
			return;
		}
	}
	printf("Invalid instruction\n");
	exit(1);
}

bool move_robot(int pos_x, int pos_y, int movement_vector[2], size_t width,
				size_t height, char *data, bool is_robot, Robot *r) {
	// Recurisvely move the robot and the boxes in front of it
	if (!is_robot) {
		// If the current tiles is not a robot, then:
		// - If it is a wall, return false as the robot cannot move
		// - If it is an empty space, return true as somewhere after a few boxes
		// there is an empty space
		// - If it is a box, check if there is an empty space ahead and move the
		// box if possible
		if (pos_x < 0 || pos_x >= (int)width || pos_y < 0 ||
			pos_y >= (int)height) {
			printf("Robot out of bounds\n");
			exit(1);
		}
		int pos = pos_y * width + pos_x;
		if (data[pos] == '#') {
			return false;
		} else if (data[pos] == '.') {
			return true;
		} else if (data[pos] == 'O') {
			int new_x = pos_x + movement_vector[0];
			int new_y = pos_y + movement_vector[1];
			bool can_move = move_robot(new_x, new_y, movement_vector, width,
									   height, data, false, r);
			if (can_move) {
				int new_pos = new_y * width + new_x;
				data[new_pos] = 'O';
				data[pos] = '.';
				return true;
			} else {
				return false;
			}
		} else {
			printf("Invalid data\n");
			exit(1);
		}
	} else {
		// If the current tile is a robot, then:
		// - Check if there is an empty space ahead and move the
		// robot if possible

		int new_pos_x = r->x + movement_vector[0];
		int new_pos_y = r->y + movement_vector[1];
		bool can_move = move_robot(new_pos_x, new_pos_y, movement_vector, width,
								   height, data, false, r);
		if (can_move) {
			r->x = new_pos_x;
			r->y = new_pos_y;
			return true;
		} else {
			return false;
		}
	}
}

void execute_instructions(char *data, char *instructions, size_t width,
						  size_t height) {
	// Identify robot and remove it from the data
	Robot r;
	get_robot_pos(data, width, height, &r);
	data[r.y * width + r.x] = '.';

	// Execute each instruction
	while (*instructions != '\0') {
		// Skip newlines
		if (*instructions == '\n') {
			instructions++;
			continue;
		}
		// Get the change in position for the given instruction
		int movement_vector[2];
		get_movement_vector(*instructions, movement_vector);

		// Try moving the robot
		move_robot(-1, -1, movement_vector, width, height, data, true, &r);
		// print_data(data, width, height, r);
		instructions++;
	}
}

llu sum_coords(char *data, size_t width, size_t height) {
	llu sum = 0;
	for (size_t i = 0; i < width * height; i++) {
		if (data[i] == 'O') {
			sum += i % width + i / width * 100;
		}
	}
	return sum;
}

llu part1() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);

	// Split input into instructions and data
	char *instructions = strstr(data, "\n\n");
	instructions[1] = '\0';
	instructions += 2;

	size_t data_size = (instructions - 1) - data;

	// Transform data into a 2D array
	size_t width, height;
	data =
		remove_newlines_and_null_terminator(data, &data_size, &width, &height);

	execute_instructions(data, instructions, width, height);

	llu sum = sum_coords(data, width, height);

	free(data);
	return sum;
}

bool move_robot2(int pos_x, int pos_y, int movement_vector[2], size_t width,
				 size_t height, char *data, bool is_robot, Robot *r,
				 bool actually_move) {
	// Recurisvely move the robot and the boxes in front of it
	if (!is_robot) {
		// If the current tiles is not a robot, then:
		// - If it is a wall, return false as the robot cannot move
		// - If it is an empty space, return true as somewhere after a few boxes
		// there is an empty space
		// - If it is a box, check if there is an empty space ahead and move the
		// box if possible
		if (pos_x < 0 || pos_x >= (int)width || pos_y < 0 ||
			pos_y >= (int)height) {
			printf("Robot out of bounds\n");
			exit(1);
		}
		int pos = pos_y * width + pos_x;
		if (data[pos] == '#') {
			return false;
		} else if (data[pos] == '.') {
			return true;
		} else if (movement_vector[1] == 0) {
			// horizontal movement, can handle boxes separately
			int new_x = pos_x + movement_vector[0];
			int new_y = pos_y + movement_vector[1];
			bool can_move = move_robot2(new_x, new_y, movement_vector, width,
										height, data, false, r, actually_move);
			if (can_move) {
				if (actually_move) {
					int new_pos = new_y * width + new_x;
					char tmp = data[new_pos];
					data[new_pos] = data[pos];
					data[pos] = tmp;
				}
				return true;
			} else {
				return false;
			}
		} else {
			// vertical movement, need  to handle boxes as pairs
			char chr = data[pos];
			if (chr != '[' && chr != ']') {
				printf("Invalid data\n");
				exit(1);
			}

			// calculate pair position
			int pair_pos_x = pos_x + (chr == '[' ? 1 : -1);
			int pair_pos_y = pos_y;
			int pair_pos = pair_pos_y * width + pair_pos_x;

			// calculate new positions
			int new_x = pos_x + movement_vector[0];
			int new_y = pos_y + movement_vector[1];
			int pair_new_x = pair_pos_x + movement_vector[0];
			int pair_new_y = pair_pos_y + movement_vector[1];

			bool can_I_move =
				move_robot2(new_x, new_y, movement_vector, width, height, data,
							false, r, actually_move);
			bool can_pair_move =
				move_robot2(pair_new_x, pair_new_y, movement_vector, width,
							height, data, false, r, actually_move);
			if (can_I_move && can_pair_move) {
				if (actually_move) {
					int new_pos = new_y * width + new_x;
					int pair_new_pos = pair_new_y * width + pair_new_x;

					char tmp = data[new_pos];
					data[new_pos] = data[pos];
					data[pos] = tmp;

					tmp = data[pair_new_pos];
					data[pair_new_pos] = data[pair_pos];
					data[pair_pos] = tmp;
				}
				return true;
			} else {
				return false;
			}
		}
	} else {
		// If the current tile is a robot, then:
		// - Check if there is an empty space ahead and move the
		// robot if possible

		int new_pos_x = r->x + movement_vector[0];
		int new_pos_y = r->y + movement_vector[1];
		bool can_move =
			move_robot2(new_pos_x, new_pos_y, movement_vector, width, height,
						data, false, r, actually_move);
		if (can_move) {
			if (actually_move) {
				r->x = new_pos_x;
				r->y = new_pos_y;
			}
			return true;
		} else {
			return false;
		}
	}
}

void execute_instructions2(char *data, char *instructions, size_t width,
						   size_t height) {
	// Identify robot and remove it from the data
	Robot r;
	get_robot_pos(data, width, height, &r);
	data[r.y * width + r.x] = '.';

	// Execute each instruction
	while (*instructions != '\0') {
		// Skip newlines
		if (*instructions == '\n') {
			instructions++;
			continue;
		}
		// Get the change in position for the given instruction
		int movement_vector[2];
		get_movement_vector(*instructions, movement_vector);

		// Try moving the robot
		bool can_move = move_robot2(-1, -1, movement_vector, width, height,
									data, true, &r, false);
		if (can_move) {
			move_robot2(-1, -1, movement_vector, width, height, data, true, &r,
						true);
		}
		// print_data(data, width, height, r);
		instructions++;
	}
}

llu sum_coords2(char *data, size_t width, size_t height) {
	llu sum = 0;
	for (size_t i = 0; i < width * height; i++) {
		if (data[i] == '[') {
			sum += i % width + i / width * 100;
		}
	}
	return sum;
}

char *widen(char *data, size_t *width, size_t *height) {
	size_t new_width = *width * 2;
	char *new_data = (char *)malloc(new_width * (*height));
	for (size_t y = 0; y < *height; y++) {
		for (size_t x = 0; x < *width; x++) {
			size_t old_index = y * *width + x;
			size_t new_index = y * new_width + x * 2;

			char chr = data[old_index];

			switch (chr) {
			case '@':
				new_data[new_index] = '@';
				new_data[new_index + 1] = '.';
				break;
			case 'O':
				new_data[new_index] = '[';
				new_data[new_index + 1] = ']';
				break;
			default:
				new_data[new_index] = chr;
				new_data[new_index + 1] = chr;
				break;
			}
		}
	}
	*width = new_width;
	return new_data;
}

llu part2() {
	// Load data and copy it into two variables
	size_t fsize; // size of file without null terminator
	char *data = load_file("input.txt", &fsize);
	char *instructions = (char *)malloc(fsize + 1); // +1 for null terminator
	memcpy(instructions, data, fsize + 1); // +1 to include null terminator

	// Set end of data to null terminator
	char *data_split = strstr(data, "\n\n");
	*data_split = '\0';
	size_t data_size = data_split - data + 1;

	// Move instruction pointer to the start of the instructions
	char *original_instructions = instructions;
	char *instruction_split = strstr(instructions, "\n\n");
	instructions = instruction_split + 2;

	// Transform data into a 2D array and widen it as per the instructions
	size_t width, height;
	data =
		remove_newlines_and_null_terminator(data, &data_size, &width, &height);
	data = widen(data, &width, &height);

	execute_instructions2(data, instructions, width, height);

	llu sum = sum_coords2(data, width, height);

	free(original_instructions);
	free(data);
	return sum;
}
