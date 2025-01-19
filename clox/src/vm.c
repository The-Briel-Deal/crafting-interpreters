#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "chunk.h"
#include "debug.h"
#include "memory.h"
#include "value.h"
#include "vm.h"

VM vm;

static void resetStack() { vm.stackTop = vm.stack; };

void initVM() {
  vm.stackSize = 0;
  vm.stackCapacity = 0;

  resetStack();
}

void freeVM() {}

void push(Value value) {
  int offsetFromStart = vm.stackTop - vm.stack;
  if (offsetFromStart + 1 >= vm.stackCapacity) {
    printf("\ncapacity: %i\nsize: %i\n", vm.stackCapacity, offsetFromStart);
    int oldCapacity = vm.stackCapacity;
    vm.stackCapacity = GROW_CAPACITY(vm.stackCapacity);
    vm.stack = GROW_ARRAY(Value, vm.stack, oldCapacity, vm.stackCapacity);
    vm.stackTop = vm.stack + offsetFromStart;
  }
  *vm.stackTop = value;
  vm.stackTop++;
}

Value pop() {
  vm.stackTop--;
  return *vm.stackTop;
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double b = pop();                                                          \
    double a = pop();                                                          \
    push(a op b);                                                              \
  } while (false)

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    printf("          ");
    for (Value *slot = vm.stack; slot < vm.stackTop; slot++) {
      printf("[ ");
      printValue(*slot);
      printf(" ]");
    }
    printf("\n");
    disassembleInstruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      push(constant);
      break;
    }
    case OP_ADD:
      BINARY_OP(+);
      break;
    case OP_SUBTRACT:
      BINARY_OP(-);
      break;
    case OP_MULTIPLY:
      BINARY_OP(*);
      break;
    case OP_DIVIDE:
      BINARY_OP(/);
      break;
    case OP_NEGATE:
      push(-pop());
      break;
    case OP_RETURN: {
      printValue(pop());
      printf("\n");
      return INTERPRET_OK;
    }
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

InterpretResult interpret(Chunk *chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}
