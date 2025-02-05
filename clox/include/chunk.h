#ifndef clox_chunk_h
#define clox_chunk_h

#include <stdint.h>

#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_NIL,
  OP_TRUE,
  OP_FALSE,
  OP_EQUAL,
  OP_GREATER,
  OP_LESS,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NOT,
  OP_NEGATE,
  OP_PRINT,
  OP_RETURN,
} OpCode;

typedef struct {
  int        count;
  int        capacity;
  uint8_t   *code;
  int       *lines;
  ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);
void freeChunk(Chunk *chunk);
void writeChunk(Chunk *chunk, uint8_t byte, int line);

/**
 * Adds a constant to a chunk and returns the index.
 *
 * @param chunk Chunk to add to.
 * @param value Value to add to the afformentioned chunk.
 * @return The index to reference this constant later.
 */
int addConstant(Chunk *chunk, Value value);

#endif
