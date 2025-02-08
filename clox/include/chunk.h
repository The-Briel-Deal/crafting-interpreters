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
  OP_GET_LOCAL,
  OP_SET_LOCAL,
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

/**
 * Initializes a `Chunk` structure, setting its fields to their default values.
 *
 * This function sets the `count` and `capacity` of the chunk to 0, and
 * initializes the `code` and `lines` arrays to `NULL`. It also initializes the
 * `constants` array using the `initValueArray` function. This is typically
 * called to prepare a `Chunk` for use before any bytes or data are written to
 * it.
 *
 * @param chunk A pointer to the `Chunk` structure to be initialized.
 *
 * @note This function does not allocate memory for the chunk's arrays. It only
 *       sets up the structure for future use. Memory allocation for arrays
 *       typically occurs when adding data to the chunk.
 */
void initChunk(Chunk *chunk);

/**
 * Frees the memory allocated for the `code`, `lines`, and `constants` arrays
 * within the given `Chunk` structure and resets the chunk to its initial state.
 *
 * This function ensures that any dynamically allocated memory for the chunk's
 * `code`, `lines`, and `constants` arrays is properly released. It then calls
 * `initChunk` to reinitialize the chunk to a clean state, ready for reuse.
 *
 * @param chunk A pointer to the `Chunk` structure to be freed and
 * reinitialized.
 *
 * @note This function assumes that the chunk's arrays were previously allocated
 *       using `GROW_ARRAY` and that `freeValueArray` handles the deallocation
 *       of the `constants` array.
 */
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
