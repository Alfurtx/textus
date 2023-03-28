#ifndef ARENA_H
#define ARENA_H

#include "common.h"

#define REGION_DEFAULT_CAPACITY (8 * 1024)

typedef struct Region Region;
typedef struct Arena Arena;
typedef struct ArenaLocal ArenaLocal;

struct Region {
	Region* next;
	usize total;
	usize used;
	u8* base;
};

struct Arena {
	Region* begin;
	Region* end;
};

struct ArenaLocal {
	Arena* arena;
	Region* region;
	usize pos;
};

Region* region_new(usize size);
void region_release(Region* region);
ArenaLocal arena_get_scratch(Arena* arena);
void arena_release_scratch(ArenaLocal* local);
void* arena_push(Arena* arena, usize size);
void arena_release(Arena* arena);
void arena_clear(Arena* arena);

#endif // ARENA_H
