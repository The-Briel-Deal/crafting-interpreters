#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
  Chunk   *chunk;
  uint8_t *ip;
  Value    stack[STACK_MAX];
  Value   *stackTop;
  Obj     *objects;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
} InterpretResult;

extern VM vm;

void            initVM();
void            freeVM();
InterpretResult interpret(const char *chunk);
void            push(Value value);
Value           pop();

/// TEST HELPERS

// TODO: I need this to run my tests rn, but I'm going to remove this once I can
// run expressions.
InterpretResult TEST_interpretChunk(Chunk *chunk);

#endif
