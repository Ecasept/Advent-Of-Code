#include "../lib/cutils.h"
#include <stdbool.h>
#include <stdlib.h>

struct Node {
	int value;
	struct Node *next;
};

typedef struct Node Node;

Node *insert_sorted(Node *node, int value) {
	Node *first = node;
	Node *prev = NULL;
	while (true) {
		if (node == NULL || node->value > value) {
			Node *new_node = malloc(sizeof(Node));
			new_node->value = value;
			new_node->next = node;

			if (prev != NULL) {
				// update previous and return first
				prev->next = new_node;
				return first;
			} else {
				// added node in beginning, no need to update previous
				// return new beginning node
				return new_node;
			}
		}
		prev = node;
		node = node->next;
	}
}

Node *dfs(size_t x, size_t y, Node *first, char *data, int columns, int rows) {
	int dirs[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
	for (int i = 0; i < 4; i++) {
		int new_x = x + dirs[i][0];
		int new_y = y + dirs[i][1];
		if (new_x < 0 || new_y < 0 || new_x >= columns || new_y >= rows) {
			// out of bounds
			continue;
		}
		int old_index = y * columns + x;
		int new_index = new_y * columns + new_x;
		if (data[new_index] - data[old_index] != 1) {
			// no increase of 1
			continue;
		}
		if (data[new_index] == '9') {
			first = insert_sorted(first, new_index);
		} else {
			first = dfs(new_x, new_y, first, data, columns, rows);
		}
	}
	return first;
}

int count_unique(Node *first) {
	int prev = -1;
	int sum = 0;
	while (first != NULL) {
		if (first->value != prev) {
			sum++;
		}
		prev = first->value;
		first = first->next;
	}
	return sum;
}

void free_linked_list(Node *first) {
	Node *old_first;
	while (first != NULL) {
		old_first = first;
		first = first->next;
		free(old_first);
	}
}

llu part1() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	size_t columns;
	size_t rows;
	data = remove_newlines_and_null_terminator(data, &fsize, &columns, &rows);

	llu sum = 0;
	for (int i = 0; i < (int)rows; i++) {
		for (int j = 0; j < (int)columns; j++) {

			if (data[i * columns + j] == '0') {
				Node *first = malloc(sizeof(Node));
				first->value = -1;
				first->next = NULL;
				sum += count_unique(dfs(j, i, first, data, columns, rows));
				free_linked_list(first);
			}
		}
	}

	free(data);
	return sum;
}

int dfs2(size_t x, size_t y, char *data, int columns, int rows) {
	int dirs[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};
	int sum = 0;
	for (int i = 0; i < 4; i++) {
		int new_x = x + dirs[i][0];
		int new_y = y + dirs[i][1];
		if (new_x < 0 || new_y < 0 || new_x >= columns || new_y >= rows) {
			// out of bounds
			continue;
		}
		int old_index = y * columns + x;
		int new_index = new_y * columns + new_x;
		if (data[new_index] - data[old_index] != 1) {
			// no increase of 1
			continue;
		}
		if (data[new_index] == '9') {
			sum++;
		} else {
			sum += dfs2(new_x, new_y, data, columns, rows);
		}
	}
	return sum;
}

llu part2() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	size_t columns;
	size_t rows;
	data = remove_newlines_and_null_terminator(data, &fsize, &columns, &rows);

	llu sum = 0;
	for (int i = 0; i < (int)rows; i++) {
		for (int j = 0; j < (int)columns; j++) {

			if (data[i * columns + j] == '0') {
				sum += dfs2(j, i, data, columns, rows);
			}
		}
	}

	free(data);
	return sum;
}
