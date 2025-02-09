#ifndef clox_compiler_h
#define clox_compiler_h

#include <limits.h>
#include <stdbool.h>

#include "chunk.h"
#include "object.h"

#define UINT8_COUNT  (UINT8_MAX + 1)
#define UINT16_COUNT (UINT16_MAX + 1)

bool compile(const char *source, Chunk *chunk);

#endif
