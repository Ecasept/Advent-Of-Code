// IMPORTANT: you need to free the returned buffer!!!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	buffer[*fsize] = '\0';

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
