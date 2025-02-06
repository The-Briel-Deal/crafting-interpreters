#ifndef clox_chunk_h
#define clox_chunk_h

#include <stdint.h>

#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_NIL,
  OP_TRUE,
  OP_FALSE,
  OP_POP,
  OP_GET_GLOBAL,
  OP_DEFINE_GLOBAL,
  OP_SET_GLOBAL,
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

/**
 * Writes a byte to the chunk's code array and associates it with the given line
 * number. If the chunk's code array is not large enough to accommodate the new
 * byte, it will be resized.
 *
 * @param chunk The chunk to which the byte will be written. This should point
 * to a valid Chunk structure that contains the code and line arrays.
 * @param byte The byte of code to be written into the chunk.
 * @param line The line number corresponding to the byte in the source code.
 *
 * This function ensures that the chunk's arrays have enough capacity to store
 * the new byte and its associated line number. If necessary, it will
 * dynamically allocate more space for both the `code` and `lines` arrays using
 * the `GROW_ARRAY` macro. After writing the byte and the line number, the
 * chunk's count is incremented.
 */
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
