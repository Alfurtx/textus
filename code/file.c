#include "file.h"

static inline usize
get_file_size(FILE* f)
{
	fseek(f, 0, SEEK_END);
	usize size = ftell(f);
	fseek(f, 0, SEEK_SET);
	return size;
}

void
read_file(const char* filepath, Buffer* buffer)
{
	FILE* f = fopen(filepath, "r");
	Assert(f, "Couldn't open file");
	usize fsize = get_file_size(f);

	if(buffer->capacity < fsize) {
		buffer->capacity = fsize;
		buffer->items = realloc(buffer->items, buffer->capacity * sizeof(char));
		Assert(buffer->items != NULL, "Realloc FAILED");
	}

	fread(buffer->items, fsize, 1, f);
	fclose(f);

	buffer->count = fsize;
	// buffer->items[fsize] = '\0';
}

char*
read_file_old(const char* filepath)
{
	FILE* f = fopen(filepath, "rb");
	Assert(f, "Couldn't open file");
	usize fsize = get_file_size(f);

	char* b;

	b = malloc(fsize);

	Assert(b, "Malloc FAILED");

	fread(b, fsize, 1, f);
	fclose(f);

	b[fsize] = '\0';
	return b;
}
