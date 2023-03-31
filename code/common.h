#ifndef COMMON_H
#define COMMON_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>

#include "config.h"

#ifdef _WIN32
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define KB(v) (1024LL * v)
#define MB(v) (1024LL * KB(v))
#define GB(v) (1024LL * MB(v))
#define TB(v) (1024LL * GB(v))

typedef int8_t       i8;
typedef int16_t      i16;
typedef int32_t      i32;
typedef int64_t      i64;
typedef uint8_t      u8;
typedef uint16_t     u16;
typedef uint32_t     u32;
typedef uint64_t     u64;
typedef unsigned int uint;
typedef size_t       usize;

#define internal static
#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof(arr[0]))
#define DEFAULT_CAPACITY 256

typedef struct {
	usize capacity;
	usize count;
	char* items;
} Buffer;

#define arr_append(arr, item)											\
	do {																\
		if((arr)->count >= (arr)->capacity) {							\
			(arr)->capacity = (arr)->capacity == 0 ? DEFAULT_CAPACITY : (arr)->capacity * 2; \
			(arr)->items = realloc((arr)->items, (arr)->capacity * sizeof(*(arr)->items)); \
			assert((arr)->items, "Realloc failed");						\
	}																	\
		(arr)->items[(arr)->count++] = (item);							\
	} while(0)

#define arr_append_many(arr, mitem, mcount)								\
	do {																\
		if((arr)->count + mcount > (arr)->capacity) {					\
			if((arr)->capacity == 0)	{								\
				(arr)->capacity = DEFAULT_CAPACITY;						\
			}															\
			while((arr)->count + mcount >= (arr)->capacity) {			\
				(arr)->capacity *= 2;									\
			}															\
			(arr)->items = realloc((arr)->items, (arr)->capacity * sizeof(*(arr)->items)); \
			assert((arr)->items, "Realloc FAILED");						\
		}																\
		memcpy((arr)->items + (arr)->count, mitem, mcount * sizeof(*(arr)->items)); \
		(arr)->count += mcount;											\
	} while(0)
#define arr_append_cstr(sb, cstr) \
    do {                          \
        const char *s = (cstr);   \
        size_t n = strlen(s);     \
        arr_append_many(sb, s, n); \
    } while (0)
#define arr_append_null(sb) arr_append_many(sb, "", 1)

#define NDEBUG
#include <assert.h>
#undef assert
#define assert(E, M) (void)((E) || (_assert(#E, __FILE__, __LINE__, M),0))
static inline void
_assert(const char* expr, const char* filename, const int line, char* message)
{
    if(!message) {
        fprintf(stderr, "[ERROR][%s:%d] %s\n", filename, line, expr);
    } else {
        fprintf(stderr, "[ERROR][%s:%d] %s\n", filename, line, message);
    }
    exit(EXIT_FAILURE);
}

#define UNIMPLEMENTED(...)                                                      \
    do {                                                                        \
        printf("%s:%d: UNIMPLEMENTED: %s \n", __FILE__, __LINE__, __VA_ARGS__); \
        exit(1);                                                                \
    } while(0)

#endif
