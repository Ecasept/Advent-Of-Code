#include "../lib/cutils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int min(int a, int b) { return a < b ? a : b; }

struct Node {
	int capacity;
	int size;
	int id;
	struct Node *next;
	struct Node *prev;
};

// Construct doubly linked list out of input data
void decode_input(char *data, struct Node **first, struct Node **last) {
	struct Node *prev = NULL;
	bool is_file = true;
	int id = 0;
	while (*data != '\n') {
		struct Node *node = malloc(sizeof(struct Node));
		node->id = is_file ? id : -1;
		node->capacity = *data - '0';
		node->size = is_file
						 ? node->capacity
						 : 0; // all ready at full capacity if isn't empty space
		node->next = NULL;
		node->prev = prev;

		if (prev != NULL) {
			prev->next = node;
		} else {
			*first = node;
		}
		prev = node;

		if (is_file) {
			id++;
		}
		is_file = !is_file;
		data++;
	}
	*last = prev;
}

void delete_node(struct Node *node) {
	if (node->next != NULL) {
		// update backwards pointer
		node->next->prev = node->prev;
	}
	// update forwards pointer
	node->prev->next = node->next;
	free(node);
}

void split_node(struct Node *node) {
	if (node->next == NULL) {
		printf("Shouldn't append to end of list\n");
		exit(1);
	}
	struct Node *second_node = malloc(sizeof(struct Node));
	second_node->id = -1;  // free space
	second_node->size = 0; // empty

	// transfer leftover capacity from node to second_node
	second_node->capacity = node->capacity - node->size;
	node->capacity = node->size;

	// insert into list
	node->next->prev = second_node;
	second_node->next = node->next;

	node->next = second_node;
	second_node->prev = node;
}

void transfer(struct Node *to, struct Node *from) {
	int space_left = to->capacity - to->size;
	int can_move = min(space_left, from->size);
	to->id = from->id;
	from->size -= can_move;
	to->size += can_move;
}

llu calc_checksum_and_free(struct Node *first) {
	llu sum = 0;

	// sum up and free linked list
	struct Node *old_first;
	llu i = 0;
	while (first != NULL) {
		if (first->id == -1) {
			// allowed in part 2
			// printf("Free space after transfer\n");
			// exit(1);
		}
		if (first->next == NULL) {
			first->capacity = first->size;
		}
		if (first->capacity != first->size) {
			// allowed in part 2
			// printf("Found unfilled node not at end of list\n");
			// exit(1);
		}

		// sum up
		llu sum_to_start = i * (i + 1) / 2;
		llu final_index = i + first->size -
						  1; // only account for occupied blocks (file->size)
		llu sum_to_end = final_index * (final_index + 1) / 2;
		llu sum_between = sum_to_end - sum_to_start + i;
		sum += sum_between * (first->id == -1 ? 0 : first->id);
		i += first->capacity;

		// free
		old_first = first;
		first = first->next;
		free(old_first);
	}
	return sum;
}

llu part1() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);

	struct Node *front;
	struct Node *back;
	decode_input(data, &front, &back);
	struct Node *first = front;

	while (true) {
		if (front == NULL || back == NULL) {
			printf(
				"Error: unexpectedly reached end/beginning of linked list\n");
			exit(1);
		}

		if (back->id == -1 || back->size == 0) {
			// free space, can delete
			back = back->prev;
			delete_node(back->next);
			if (back == front) {
				break;
			}
			continue;
		}
		if (front->size == front->capacity && front->id != -1) {
			// already filled, can skip
			front = front->next;
			if (front == back) {
				break;
			}
			continue;
		}
		if (front->id == -1 || front->id == back->id) {
			// empty space/same id, can just copy
			transfer(front, back);
		} else {
			// already contains file, needs to be split
			split_node(front);
			front = front->next;
			transfer(front, back);
		}
	}

	return calc_checksum_and_free(first);
}

llu part2() {
	size_t fsize;
	char *data = load_file("input.txt", &fsize);

	struct Node *front;
	struct Node *back;
	decode_input(data, &front, &back);
	struct Node *first = front;

	while (true) {
		if (back == NULL) {
			printf("Error: unexpectedly reached beginning of linked list\n");
			exit(1);
		}
		if (front == back) {
			// finished going through list
			break;
		}
		if ((back->id == -1 || back->size == 0)) {
			// free space, ignore
			if (back->next == NULL) {
				back = back->prev;
				delete_node(back->next);
			} else {
				back = back->prev;
			}
			if (back == front) {
				break;
			}
			continue;
		}
		while (true) {
			if (front == NULL) {
				printf("Error: unexpectedly reached end of linked list\n");
				exit(1);
			}
			if (front == back) {
				// couldn't fit element
				back = back->prev;
				break;
			}
			if (front->id == -1) {
				// free space
				if (front->capacity >= back->size) {
					transfer(front, back);
					if (front->capacity > front->size) {
						// still empty space left
						split_node(front);
					}
					break;
				} else {
					// isn't big enough
					front = front->next;
				}
			} else {
				// already occupied
				front = front->next;
			}
		}
		front = first;
	}

	return calc_checksum_and_free(first);
}
