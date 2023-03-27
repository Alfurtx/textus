#include "file.h"

static usize
GetFileSize(FILE* f)
{
	fseek(f, 0, SEEK_END);
	usize size = ftell(f);
	fseek(f, 0, SEEK_SET);
	return size;
}

char*
ReadFile(const char* filepath)
{
	FILE* f = fopen(filepath, "rb");
	assert(f, "Couldn't open file");
	usize fsize = GetFileSize(f);
	char* buf = malloc(fsize + 1);
	fread(buf, 1, fsize, f);
	assert(buf != NULL, "ReadFile alloc FAILED");
	buf[fsize] = '\0';
	fclose(f);

	return buf;
}
