#include <assert.h>

#include "helper_test.h"
#include "value.h"
#include "vm.h"

const char TEST_ARITHMETIC_EXPECT[] = "-0.821429\n";
void       testArithmetic() {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  CONSTANT(NUMBER_VAL(1.2), 123);
  CONSTANT(NUMBER_VAL(3.4), 123);
  // Pops Const 1 and Const 2, then pushes the sum of 4.6
  writeChunk(&chunk, OP_ADD, 123);

  CONSTANT(NUMBER_VAL(5.6), 123);
  // (C1 + C2) / C3 = .821~ish
  writeChunk(&chunk, OP_DIVIDE, 123);
  // -((C1 + C2) / C3) = -.821~ish
  writeChunk(&chunk, OP_NEGATE, 123);
  writeChunk(&chunk, OP_RETURN, 123);
  assert(TEST_interpretChunk(&chunk) == INTERPRET_OK);
}

const TestCase VM_TESTS[] = {
    {"testArithmetic", testArithmetic, TEST_ARITHMETIC_EXPECT},
};

const int VM_TESTS_COUNT = sizeof(VM_TESTS) / sizeof(TestCase);
