#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_RETURN,
} OpCode;

typedef struct {
  int lineNum;
  int count;
} LineNum;

typedef struct {
  int count;
  int capacity;
  uint8_t *code;
  int lineCount;
  int lineCapacity;
  LineNum *lines;
  ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);
int getLine(Chunk *chunk, int offset);

/**
 * Adds a constant to a chunk and returns the index.
 *
 * @param chunk Chunk to add to.
 * @param value Value to add to the afformentioned chunk.
 * @return The index to reference this constant later.
 */
int addConstant(Chunk *chunk, Value value);

#endif
