#ifndef clox_helper_test_h
#define clox_helper_test_h

#include <stdbool.h>

#define CONSTANT(val, line)                                                    \
  do {                                                                         \
    int constant = addConstant(&chunk, val);                                   \
    writeChunk(&chunk, OP_CONSTANT, line);                                     \
    writeChunk(&chunk, constant, line);                                        \
  } while (false)

typedef struct {
  char *name;
  void (*fn)();
  const char *expect;
} TestCase;

#endif
