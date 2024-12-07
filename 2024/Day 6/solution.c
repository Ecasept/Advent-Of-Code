#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// IMPORTANT: you need to free the returned buffer!!!
char *load_file(const char *filename, size_t *fsize) {
	FILE *fptr;

	fptr = fopen(filename, "r");

	if (fptr == NULL) {
		perror("File not found");
		exit(1);
	}

	fseek(fptr, 0, SEEK_END);
	*fsize = ftell(fptr);
	rewind(fptr);

	char *buffer = (char *)malloc(*fsize + 1); // +1 for null terminator
	if (buffer == NULL) {
		perror("Failed to allocate memory for file");
		exit(1);
	}

	size_t read_size = fread(buffer, 1, *fsize, fptr);
	if (read_size != *fsize) {
		perror("Could not read entire file");
		free(buffer);
		fclose(fptr);
		exit(1);
	}

	buffer[*fsize] = '\0'; // end string with null terminator

	fclose(fptr);
	return buffer;
}

char *remove_newlines_and_null_terminator(char *old_data, size_t *data_size,
										  size_t *columns, size_t *rows) {
	char *first_newline_ptr = strchr(old_data, '\n');
	if (first_newline_ptr == NULL) {
		perror("No newline found");
		exit(1);
	}
	*columns = first_newline_ptr - old_data;
	*rows = ((*data_size)) / ((*columns) + 1); // -1 for \0, +1 for every \n

	// update new data size
	*data_size = (*columns) * (*rows);

	char *new_data = (char *)malloc(sizeof(char) * (*data_size));

	for (int i = 0; i < (*rows); i++) {
		for (int j = 0; j < (*columns); j++) {
			int old_index = i * ((*columns) + 1) + j; // +1 for \n
			int new_index = i * (*columns) + j;
			new_data[new_index] = old_data[old_index];
		}
	}
	free(old_data);
	return new_data;
}

enum Direction { UP, RIGHT, DOWN, LEFT };
struct Position {
	size_t x;
	size_t y;
	enum Direction dir;
};

// return true if outside
bool go_forward(struct Position *pos, size_t columns, size_t rows) {
	switch (pos->dir) {
	case UP:
		pos->y--;
		break;
	case RIGHT:
		pos->x++;
		break;
	case DOWN:
		pos->y++;
		break;
	case LEFT:
		pos->x--;
		break;
	default:
		printf("Invalid dir");
		exit(1);
	}
	if (pos->y < 0 || pos->x < 0 || pos->y >= rows || pos->x >= columns) {
		return true;
	}
	return false;
}

void turn_right(struct Position *pos) { pos->dir = (pos->dir + 1) % 4; }

int *visited_positions;

int find_positions(char *data, size_t columns, size_t rows,
				   struct Position *pos) {
	int guard_positions = 0;
	while (true) {
		struct Position old_pos = *pos;
		bool is_outside = go_forward(pos, columns, rows);
		if (is_outside) {
			return guard_positions;
		}
		size_t index = pos->y * columns + pos->x;
		if (data[index] == '#') {
			turn_right(&old_pos);
			return guard_positions +
				   find_positions(data, columns, rows, &old_pos);
		}

		// yeah no way i'm implementing a hashmap in c O(n^2) go brrrrrrr
		for (int i = 0; i < columns * rows * 2; i += 2) {
			if (visited_positions[i] == -1) {
				// position has not been visited
				visited_positions[i] = pos->x;
				visited_positions[i + 1] = pos->y;
				guard_positions++;
				break;
			} else {
				if (visited_positions[i] == pos->x &&
					visited_positions[i + 1] == pos->y) {
					// position has already been visited
					break;
				}
			}
		}
	}
}

unsigned long long part1() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	size_t columns;
	size_t rows;
	data = remove_newlines_and_null_terminator(data, &fsize, &columns, &rows);

	visited_positions = (int *)malloc(sizeof(int) * columns * rows * 2);
	for (int i = 0; i < columns * rows * 2; i++) {
		visited_positions[i] = -1;
	}

	const char positions[] = {'^', '>', 'v', '<'};
	const int pos_count = sizeof(positions) / sizeof(char);

	int guard_positions = 0;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			int index = i * columns + j;
			for (int list_index = 0; list_index < pos_count; list_index++) {
				if (positions[list_index] == data[index]) {
					struct Position pos = {j, i, list_index};
					guard_positions = find_positions(data, columns, rows, &pos);
					goto end;
				}
			}
		}
	}
end:
	free(data);
	free(visited_positions);
	return guard_positions;
}

int *visited_positions2;

bool find_positions2(char *data, size_t columns, size_t rows,
					 struct Position *pos) {
	while (true) {
		struct Position old_pos = *pos;
		bool is_outside = go_forward(pos, columns, rows);
		if (is_outside) {
			return false;
		}
		size_t index = pos->y * columns + pos->x;
		if (data[index] == '#') {
			turn_right(&old_pos);
			return find_positions2(data, columns, rows, &old_pos);
		}

		size_t offset = ((columns) * (rows)) * pos->dir;

		if (visited_positions2[index + offset] == -1) {
			// position has not been visited
			visited_positions2[index + offset] = 0;
		} else {
			// position has already been visited
			return true;
		}
	}
}

void reset_visited_positions(size_t cols, size_t rows) {
	for (int i = 0; i < cols * rows * 4; i++) {
		visited_positions2[i] = -1;
	}
}

// let's go O(n^3)
unsigned long long part2() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	size_t columns;
	size_t rows;
	data = remove_newlines_and_null_terminator(data, &fsize, &columns, &rows);

	visited_positions2 = (int *)malloc(sizeof(int) * columns * rows * 4);
	reset_visited_positions(columns, rows);

	const char positions[] = {'^', '>', 'v', '<'};
	const int pos_count = sizeof(positions) / sizeof(char);

	struct Position pos;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			int index = i * columns + j;
			for (int list_index = 0; list_index < pos_count; list_index++) {
				if (positions[list_index] == data[index]) {
					pos.x = j;
					pos.y = i;
					pos.dir = list_index;
					goto end_guard;
				}
			}
		}
	}
end_guard:

	visited_positions = (int *)malloc(sizeof(int) * columns * rows * 2);
	for (int i = 0; i < columns * rows * 2; i++) {
		visited_positions[i] = -1;
	}
	struct Position pos_copy = pos;
	find_positions(data, columns, rows, &pos_copy);

	int loop_count = 0;

	for (int i = 0; i < columns * rows * 2; i += 2) {
		int x = visited_positions[i];
		int y = visited_positions[i + 1];
		if (x == -1) {
			// end of visited positions reached
			break;
		}
		char prev;

		int index = y * columns + x;

		if (data[index] == '#') {
			// skip if already obstructed
			continue;
		}

		prev = data[index];
		data[index] = '#';
		struct Position pos_copy = pos;
		loop_count += find_positions2(data, columns, rows, &pos_copy);
		reset_visited_positions(columns, rows);

		data[index] = prev;
		// enable for output while brute forcing
		// printf("%d %d %d %d\n", i, j, index, fsize);
	}

	free(data);
	free(visited_positions);
	free(visited_positions2);
	return loop_count;
}
