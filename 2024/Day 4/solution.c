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

    char *buffer = malloc(*fsize + 1); // +1 for null terminator
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

// direction: true for forward, false for backward
// step: 0 for X, 1 for M, 2 for A, 3 for S
// type: 0 for row, 1 for column, 2 for diagonal top left to bottom right, 3 for
// diagonal top right to bottom left
bool check_for_match(char *data, int index, int columns, int rows,
                     bool direction, int step, int type) {
    if (index < 0 || index >= (columns + 1) * rows) {
        return false;
    }

    const char pattern[] = {'X', 'M', 'A', 'S'};
    int adjusted_step =
        direction ? step : 3 - step; // reverse step if going backwards

    if (adjusted_step >= 4 || adjusted_step < 0) {
        perror("Invalid step");
        exit(1);
    }
    if (data[index] == pattern[adjusted_step]) {
        if (step == 3) {
            return true;
        }
        switch (type) {
        case 0:
            return check_for_match(data, index + 1, columns, rows, direction,
                                   step + 1, type);
        case 1:
            return check_for_match(data, index + columns + 1, columns, rows,
                                   direction, step + 1, type);
        case 2:
            return check_for_match(data, index + (columns + 1) + 1, columns,
                                   rows, direction, step + 1, type);
        case 3:
            return check_for_match(data, index + (columns + 1) - 1, columns,
                                   rows, direction, step + 1, type);
        default:
            perror("Invalid type");
            exit(1);
        }
    }
    return false;
}

unsigned long long part1() {
    size_t fsize;
    char *data = load_file("input.txt", &fsize);
    char *first_newline_ptr = strchr(data, '\n');
    if (first_newline_ptr == NULL) {
        perror("No newline found");
        exit(1);
    }
    int columns = first_newline_ptr - data;
    // fsize does not include null terminator
    // assume that the file ends with a newline
    // columns + 1 because we want to include the newline
    int rows = fsize / (columns + 1);
    int matches = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int index = (columns + 1) * i + j;
            matches += check_for_match(data, index, columns, rows, true, 0, 0);
            matches += check_for_match(data, index, columns, rows, false, 0, 0);
            matches += check_for_match(data, index, columns, rows, true, 0, 1);
            matches += check_for_match(data, index, columns, rows, false, 0, 1);
            matches += check_for_match(data, index, columns, rows, true, 0, 2);
            matches += check_for_match(data, index, columns, rows, false, 0, 2);
            matches += check_for_match(data, index, columns, rows, true, 0, 3);
            matches += check_for_match(data, index, columns, rows, false, 0, 3);
        }
    }

    free(data);
    return matches;
}

bool check_for_match2(char *data, int index, int columns, int rows,
                      bool direction, int step, int type) {
    if (index < 0 || index >= (columns + 1) * rows) {
        return false;
    }

    const char pattern[] = {'M', 'A', 'S'};
    const int pattern_length = sizeof(pattern) / sizeof(pattern[0]);

    int adjusted_step = direction ? step
                                  : (pattern_length - 1) -
                                        step; // reverse step if going backwards

    if (adjusted_step >= pattern_length || adjusted_step < 0) {
        perror("Invalid step");
        exit(1);
    }
    if (data[index] == pattern[adjusted_step]) {
        if (step == (pattern_length - 1)) {
            return true;
        }
        switch (type) {
        case 0:
            return check_for_match2(data, index + 1, columns, rows, direction,
                                    step + 1, type);
        case 1:
            return check_for_match2(data, index + columns + 1, columns, rows,
                                    direction, step + 1, type);
        case 2:
            return check_for_match2(data, index + (columns + 1) + 1, columns,
                                    rows, direction, step + 1, type);
        case 3:
            return check_for_match2(data, index + (columns + 1) - 1, columns,
                                    rows, direction, step + 1, type);
        default:
            perror("Invalid type");
            exit(1);
        }
    }
    return false;
}

unsigned long long part2() {
    size_t fsize;
    char *data = load_file("input.txt", &fsize);
    char *first_newline_ptr = strchr(data, '\n');
    if (first_newline_ptr == NULL) {
        perror("No newline found");
        exit(1);
    }
    int columns = first_newline_ptr - data;
    // fsize does not include null terminator
    // assume that the file ends with a newline
    // columns + 1 because we want to include the newline
    int rows = fsize / (columns + 1);
    int matches = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            int index = (columns + 1) * i + j;
            char chr = data[index];
            if (chr == 'A') {
                int topleft = index - (columns + 1) - 1;
                int topright = index - (columns + 1) + 1;
                int tmpmatches = 0;
                tmpmatches +=
                    check_for_match2(data, topleft, columns, rows, true, 0, 2);
                tmpmatches +=
                    check_for_match2(data, topleft, columns, rows, false, 0, 2);
                tmpmatches +=
                    check_for_match2(data, topright, columns, rows, true, 0, 3);
                tmpmatches += check_for_match2(data, topright, columns, rows,
                                               false, 0, 3);
                if (tmpmatches == 2) {
                    matches++;
                }
            }
        }
    }

    free(data);
    return matches;
}
