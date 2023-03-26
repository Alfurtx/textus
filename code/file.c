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
ReadFile(Arena* arena, const char* filepath)
{
	FILE* f = fopen(filepath, "rb");
	assert(f, "Couldn't open file");
	usize fsize = GetFileSize(f);
	char* buf = ArenaPush(arena, fsize);
	fread(buf, 1, fsize, f);
	buf[fsize] = '\0';
	fclose(f);

	return buf;
}
