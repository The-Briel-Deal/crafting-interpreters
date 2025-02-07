#ifndef clox_compiler_h
#define clox_compiler_h

#include <stdbool.h>
#include <limits.h>

#include "chunk.h"
#include "object.h"


#define UINT8_COUNT (UINT8_MAX + 1)

bool compile(const char *source, Chunk *chunk);

#endif
