#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memory.h"

typedef struct {
  int size;
	bool freed;
} MemChunkMetadata;

void *realloc(void *__ptr, size_t __size) {
  MemChunkMetadata *p = sbrk(__size + sizeof(MemChunkMetadata));
  p->size = __size;
  void *newMemPtr = p + 1;

  if (__ptr) {
    MemChunkMetadata *oldMetadata = ((MemChunkMetadata *)__ptr) - 1;
    int oldSize = oldMetadata->size;
    memcpy(newMemPtr, __ptr, oldSize);
  }

	return newMemPtr;
}

void free (void *__ptr) {
	(((MemChunkMetadata*)__ptr)-1)->freed = true;
};

void *reallocate(void *pointer, size_t oldSize, size_t newSize) {
  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void *result = realloc(pointer, newSize);
  if (result == NULL)
    exit(1);
  return result;
}
