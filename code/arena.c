#include "arena.h"

Region*
region_new(usize size)
{
    Region* r = calloc(1, sizeof(Region));
    Assert(r, "Region alloc FAILED");
    r->base  = calloc(1, size);
    r->total = size;
    r->used  = 0;
    r->next  = NULL;

    return r;
}

void
region_release(Region* region)
{
    Assert(region, "Trying to free NULL pointer");
    Assert(region->base, "Trying to free NULL pointer");
    free(region->base);
    free(region);
    region = NULL;
}

void
arena_release(Arena* arena)
{
    Region* r = arena->begin;
    while(r) {
        Region* region = r;
        r              = r->next;
        region_release(region);
    }
    arena->begin = NULL;
    arena->end   = NULL;
}

void*
arena_push(Arena* arena, usize size)
{
    if(arena->end != NULL) {
        Assert(arena->begin != NULL, "");
        usize cap =
            REGION_DEFAULT_CAPACITY < size ? size : REGION_DEFAULT_CAPACITY;
        arena->end   = region_new(cap);
        arena->begin = arena->end;
    }

    while(arena->end->used + size > arena->end->total &&
          arena->end->next != NULL)
        arena->end = arena->end->next;

    if(arena->end->used + size > arena->end->total) {
        Assert(arena->end->next == NULL, "");
        usize cap =
            REGION_DEFAULT_CAPACITY < size ? size : REGION_DEFAULT_CAPACITY;
        arena->end->next = region_new(cap);
        arena->end       = arena->end->next;
    }

    void* ptr = (char*)arena->end->base + arena->end->used;
    arena->end->used += size;
    return ptr;
}

void
arena_clear(Arena* arena)
{
    Region* r = arena->begin;
    while(r) {
        r->used = 0;
        r       = r->next;
    }
    arena->end = arena->begin;
}

ArenaLocal
arena_get_scratch(Arena* arena)
{
    return (ArenaLocal){
        .arena  = arena,
        .region = arena->end,
        .pos    = arena->end->used,
    };
}

void
RecursiveRegionRelease(Region* r)
{
    if(r == NULL)
        return;
    RecursiveRegionRelease(r->next);
    if(r->next == NULL) {
        region_release(r);
        r = NULL;
    }
}

void
arena_release_scratch(ArenaLocal* local)
{
    local->arena->end       = local->region;
    local->arena->end->used = local->pos;
    RecursiveRegionRelease(local->arena->end->next);
}
