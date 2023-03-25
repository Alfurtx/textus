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

inline Region* RegionNew(usize size);
inline void RegionRelease(Region* region);
inline ArenaLocal ArenaGetScratch(Arena* arena);
inline void AreanReleaseScratch(ArenaLocal* local);
inline void* ArenaPush(Arena* arena, usize size);
inline void ArenaRelease(Arena* arena);
inline void ArenaClear(Arena* arena);

#ifdef ARENA_IMPLEMENTATION

inline Region*
RegionNew(usize size)
{
	Region* r = calloc(1, sizeof(Region));
	assert(r, "Region alloc FAILED");
	r->base = calloc(1, size);
	r->total = size;
	r->used = 0;
	r->next = NULL;

	return r;
}

inline void
RegionRelease(Region* region)
{
	assert(region, "Trying to free NULL pointer");
	assert(region->base, "Trying to free NULL pointer");
	free(region->base);
	free(region);
	region = NULL;
}

inline void
ArenaRelease(Arena* arena)
{
	Region* r = arena->begin;
	while(r) {
		Region* region = r;
		r = r->next;
		RegionRelease(region);
	}
	arena->begin = NULL;
	arena->end = NULL;
}

inline void*
ArenaPush(Arena* arena, usize size)
{
	if(arena->end != NULL) {
		assert(arena->begin != NULL, "");
		usize cap = REGION_DEFAULT_CAPACITY < size ? size : REGION_DEFAULT_CAPACITY;
		arena->end = RegionNew(cap);
		arena->begin = arena->end;
	}

	while(arena->end->used + size > arena->end->total && arena->end->next != NULL)
		arena->end = arena->end->next;

	if(arena->end->used + size > arena->end->total) {
		assert(arena->end->next == NULL, "");
		usize cap = REGION_DEFAULT_CAPACITY < size ? size : REGION_DEFAULT_CAPACITY;
		arena->end->next = RegionNew(cap);
		arena->end = arena->end->next;
	}

	void* ptr = (char*) arena->end->base + arena->end->used;
	arena->end->used += size;
	return ptr;
}

inline void
ArenaClear(Arena* arena)
{
	Region* r = arena->begin;
	while(r) {
		r->used = 0;
		r = r->next;
	}
	arena->end = arena->begin;
}

inline ArenaLocal
ArenaGetScratch(Arena* arena)
{
	return (ArenaLocal){
		.arena = arena,
		.region = arena->end,
		.pos = arena->end->used,
	};
}

inline void
RecursiveRegionRelease(Region* r)
{
	if(r == NULL) return;
	RecursiveRegionRelease(r->next);
	if(r->next == NULL) {
		RegionRelease(r);
		r = NULL;
	}
}

inline void
AreanReleaseScratch(ArenaLocal* local)
{
	local->arena->end = local->region;
	local->arena->end->used = local->pos;
	RecursiveRegionRelease(local->arena->end->next);
}

#endif // ARENA_IMPLEMENTATION
#endif // ARENA_H
