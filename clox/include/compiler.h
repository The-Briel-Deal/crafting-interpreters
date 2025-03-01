#ifndef clox_compiler_h
#define clox_compiler_h

#include <limits.h>
#include <stdbool.h>

#include "chunk.h"
#include "object.h"

#define UINT8_COUNT (UINT8_MAX + 1)

ObjFunction *compile(const char *source);
void markCompilerRoots();

#endif
