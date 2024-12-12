#include "../lib/cutils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_VISITED_BFS 4
#define CAN_BE_NEW_AREA 5
#define BELONGS_TO_AREA 6

struct QueueNode {
	int x;
	int y;
	struct QueueNode *next;
};

typedef struct QueueNode QueueNode;

QueueNode *queue_get(QueueNode *head) {
	QueueNode *node = head;
	head = head->next;
	free(node);
	return head;
}

QueueNode *queue_push(QueueNode *tail, int x, int y) {
	QueueNode *node = malloc(sizeof(QueueNode));
	node->x = x;
	node->y = y;
	node->next = NULL;

	tail->next = node;
	return node;
}

static inline bool queue_is_empty(QueueNode *head) { return head == NULL; }

void bfs(char *data, int start_x, int start_y, int visited[][7], size_t columns,
		 size_t rows, int *perimeter, int *area) {

	QueueNode *head = malloc(sizeof(QueueNode));
	QueueNode *tail = head;
	head->x = start_x;
	head->y = start_y;
	head->next = NULL;
	int start_index = start_y * columns + start_x;

	int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

	*perimeter = 0;
	*area = 0;

	visited[start_index][CAN_BE_NEW_AREA] = false;
	visited[start_index][LAST_VISITED_BFS] = start_index;
	visited[start_index][BELONGS_TO_AREA] = start_index;

	char start_char = data[start_y * columns + start_x];

	while (!queue_is_empty(head)) {
		(*area)++;

		int x = head->x;
		int y = head->y;

		// check all 4 directions and add to queue if not visited
		for (int dir = 0; dir < 4; dir++) {
			int new_x = x + directions[dir][0];
			int new_y = y + directions[dir][1];
			int new_index = new_y * columns + new_x;
			if (new_x < 0 || new_x >= (int)columns || new_y < 0 ||
				new_y >= (int)rows) {
				// count as perimeter if out of bounds
				(*perimeter)++;
				// printf("Out of bounds perimeter at %d %d\n", new_x, new_y);
				continue;
			}
			if (data[new_index] != start_char) {
				if (visited[new_index][dir] == start_index) {
					// visited by same direction in this bfs
					continue;
				}
				// count as perimeter if not same character
				(*perimeter)++;
				// printf("Different character perimeter at %d %d\n", new_x,
				// 	   new_y);
				visited[new_index][LAST_VISITED_BFS] = start_index;
				visited[new_index][dir] = start_index;
				continue;
			} else {
				// if visited at all by same bfs
				if (visited[new_index][4] == start_index) {
					continue;
				}

				visited[new_index][CAN_BE_NEW_AREA] = false;
				visited[new_index][LAST_VISITED_BFS] = start_index;
				visited[new_index][dir] = start_index;
				visited[new_index][BELONGS_TO_AREA] = start_index;
				tail = queue_push(tail, new_x, new_y);
			}
		}
		head = queue_get(head);
	}
}

llu run_bfs(char *data, size_t columns, size_t rows, int visited[][7],
			int start_x, int start_y) {
	int perimeter;
	int area;
	bfs(data, start_x, start_y, visited, columns, rows, &perimeter, &area);
	// printf("Area: %d, Perimeter: %d\n", area, perimeter);
	return area * perimeter;
}

llu part1() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	// remove newlines
	size_t columns, rows;
	data = remove_newlines_and_null_terminator(data, &fsize, &columns, &rows);

	// first is is global_visited, second is bfs_run_visited
	int visited[columns * rows][7];
	for (size_t i = 0; i < columns * rows; i++) {
		visited[i][0] = -1;					// last bfs that visited from bottom
		visited[i][1] = -1;					// last bfs that visited from left
		visited[i][2] = -1;					// last bfs that visited from top
		visited[i][3] = -1;					// last bfs that visited from right
		visited[i][LAST_VISITED_BFS] = -1;	// last bfs that visited at all
		visited[i][CAN_BE_NEW_AREA] = true; // can be start of new area
		visited[i][BELONGS_TO_AREA] = -1;	// area that it belongs to
	}

	llu sum = 0;

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < columns; j++) {
			if (!visited[i * columns + j][CAN_BE_NEW_AREA]) {
				continue;
			}
			sum += run_bfs(data, columns, rows, visited, j, i);
		}
	}

	free(data);
	return sum;
}

llu count_corners(int visited[][7], int x, int y, size_t columns, size_t rows,
				  int start_index) {
	int directions[4][2] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

	int corner_count = 0;
	for (int dir = 0; dir < 4; dir++) {
		int dir_after = (dir + 1) % 4;
		int new_x1 = x + directions[dir][0];
		int new_y1 = y + directions[dir][1];
		int new_x2 = x + directions[dir_after][0];
		int new_y2 = y + directions[dir_after][1];
		int new_x3 = x + directions[dir][0] + directions[dir_after][0];
		int new_y3 = y + directions[dir][1] + directions[dir_after][1];

		bool one = false;
		bool two = false;
		bool three = false;

		if (new_x1 < 0 || new_x1 >= (int)columns || new_y1 < 0 ||
			new_y1 >= (int)rows) {
			one = true;
		} else {
			if (visited[new_y1 * columns + new_x1][BELONGS_TO_AREA] !=
				start_index) {
				one = true;
			}
		}
		if (new_x2 < 0 || new_x2 >= (int)columns || new_y2 < 0 ||
			new_y2 >= (int)rows) {
			two = true;
		} else {
			if (visited[new_y2 * columns + new_x2][BELONGS_TO_AREA] !=
				start_index) {
				two = true;
			}
		}
		if (new_x3 < 0 || new_x3 >= (int)columns || new_y3 < 0 ||
			new_y3 >= (int)rows) {
			three = true;
		} else {
			if (visited[new_y3 * columns + new_x3][BELONGS_TO_AREA] !=
				start_index) {
				three = true;
			}
		}
		if (one && two) {
			//  e
			//  xe
			//
			// x = current point
			// e = empty
			// convex corner
			corner_count++;
		}
		if (!one && !two && three) {
			//  ye
			//  xy
			//
			// x = current point
			// y = point in area
			// e = empty
			// concave corner
			corner_count++;
		}
	}
	// printf("Corner count at %d %d: %d\n", x, y, corner_count);
	// if ((x + 1) % columns == 0) {
	// 	printf("\n");
	// }
	return corner_count;
}

llu run_bfs2(char *data, size_t columns, size_t rows, int visited[][7],
			 int start_x, int start_y) {
	int perimeter;
	int area;
	bfs(data, start_x, start_y, visited, columns, rows, &perimeter, &area);
	int start_index = start_y * columns + start_x;
	int corner_count = 0;
	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < columns; j++) {
			if (visited[i * columns + j][BELONGS_TO_AREA] == start_index) {
				corner_count +=
					count_corners(visited, j, i, columns, rows, start_index);
			}
		}
	}
	// printf("Area: %d, Perimeter: %d, Corners: %d\n", area, perimeter,
	// 	   corner_count);
	return corner_count * area;
}

llu part2() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);
	// remove newlines
	size_t columns, rows;
	data = remove_newlines_and_null_terminator(data, &fsize, &columns, &rows);

	// first is is global_visited, second is bfs_run_visited
	int visited[columns * rows][7];
	for (size_t i = 0; i < columns * rows; i++) {
		visited[i][0] = -1;					// last bfs that visited from bottom
		visited[i][1] = -1;					// last bfs that visited from left
		visited[i][2] = -1;					// last bfs that visited from top
		visited[i][3] = -1;					// last bfs that visited from right
		visited[i][LAST_VISITED_BFS] = -1;	// last bfs that visited at all
		visited[i][CAN_BE_NEW_AREA] = true; // can be start of new area
		visited[i][BELONGS_TO_AREA] = -1;	// area that it belongs to
	}

	llu sum = 0;

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < columns; j++) {
			if (!visited[i * columns + j][CAN_BE_NEW_AREA]) {
				continue;
			}
			sum += run_bfs2(data, columns, rows, visited, j, i);
		}
	}

	free(data);
	return sum;
}
