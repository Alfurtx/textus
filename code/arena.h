#ifndef ARENA_H
#define ARENA_H

#include "common.h"

#define REGION_DEFAULT_CAPACITY (8 * 1024)

typedef struct Region Region;
typedef struct Arena Arena;
typedef struct ArenaLocal ArenaLocal;

struct Region {
	Region* next;
	usize   total;
	usize   used;
	void*   base;
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

Region* RegionNew(usize size);
void RegionRelease(Region* region);
ArenaLocal ArenaGetScratch(Arena* arena);
void ArenaReleaseScratch(ArenaLocal* local);
void* ArenaPush(Arena* arena, usize size);
void ArenaRelease(Arena* arena);
void ArenaClear(Arena* arena);

#endif // ARENA_H
