#ifndef FILE_H
#define FILE_H

#include "common.h"

// NOTE(fonsi): YOU have to free the given pointer later
char* ReadFile(const char* filepath);

#endif
