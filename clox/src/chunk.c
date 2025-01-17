#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "chunk.h"
#include "memory.h"
#include "value.h"

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->lineCount = 0;
  chunk->lineCapacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
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
  }
  if (chunk->lineCapacity < chunk->lineCount + 1) {
    int oldCapacity = chunk->lineCapacity;
    chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
    chunk->lines =
        GROW_ARRAY(LineNum, chunk->lines, oldCapacity, chunk->lineCapacity);
  }

  chunk->code[chunk->count] = byte;

  if (chunk->lineCount > 0 && line == chunk->lines[chunk->lineCount - 1].lineNum) {
    // Incr last line
    chunk->lines[chunk->lineCount - 1].count++;
  } else {
    // Add new line.
    chunk->lines[chunk->lineCount].lineNum = line;
    chunk->lines[chunk->lineCount].count = 1;
    chunk->lineCount++;
  }
  chunk->count++;
}

int getLine(Chunk *chunk, int offset) {
  int currLineIndex = 0;
  int tillNext = chunk->lines[currLineIndex].count;
  for (int i = 0; i < offset; i++) {
    assert(currLineIndex <= chunk->lineCount);
    tillNext--;
    if (tillNext == 0) {
      currLineIndex++;
      tillNext = chunk->lines[currLineIndex].count;
    }
  }
  return chunk->lines[currLineIndex].lineNum;
}

int addConstant(Chunk *chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}
