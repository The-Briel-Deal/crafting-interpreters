#include <stdint.h>
#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

void initChunk(Chunk *chunk) {
  chunk->count    = 0;
  chunk->capacity = 0;
  chunk->code     = NULL;
  chunk->lines    = NULL;
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count]  = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}

static int containsValue(Chunk *chunk, Value value) {
  for (int i = 0; i < chunk->constants.count; i++) {
    Value currConst = chunk->constants.values[i];
    if (valuesEqual(currConst, value)) {
      return i;
    }
  }
  // -1 means not found.
  return -1;
}

int addConstant(Chunk *chunk, Value value) {
  int existingValue = containsValue(chunk, value);

  if (existingValue != -1) {
    return existingValue;
  }

  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}
