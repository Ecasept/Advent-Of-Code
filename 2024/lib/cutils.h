#include <stddef.h>

typedef unsigned long long llu;

char *load_file(const char *filename, size_t *fsize);

char *remove_newlines_and_null_terminator(char *old_data, size_t *data_size,
										  size_t *columns, size_t *rows);
