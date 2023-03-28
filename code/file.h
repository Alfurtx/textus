#ifndef FILE_H
#define FILE_H

#include "common.h"

void read_file(const char* filepath, Buffer* buffer);
char* read_file_old(const char* filepath); // DEPRECATED

#endif
