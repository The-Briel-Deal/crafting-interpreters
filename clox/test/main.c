#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "chunk.h"
#include "debug.h"
#include "vm.h"

static bool runTest(char *testName, void (*testCase)(), char *expect);

static void testWriteChunk();
static void testArithmetic();

static char TEST_ARITHMETIC_C15_1_1_EXPECT[];
static void testArithmeticC15_1_1();

static char TEST_ARITHMETIC_C15_1_2_EXPECT[];
static void testArithmeticC15_1_2();

int main(int argc, char *argv[]) {
  printf("Starting Tests (:\n");

  char testWriteChunkExpect[] = "== test chunk ==\n"
                                "0000  123 OP_CONSTANT         0 '1.2'\n"
                                "0002    | OP_RETURN\n";
  assert(runTest("testWriteChunk", testWriteChunk, testWriteChunkExpect));

  char testArithmeticExpect[] = "-0.821429\n";
  assert(runTest("testArithmetic", testArithmetic, testArithmeticExpect));

  assert(runTest("testArithmeticC15_1_1", testArithmeticC15_1_1,
                 TEST_ARITHMETIC_C15_1_1_EXPECT));

  assert(runTest("testArithmeticC15_1_2", testArithmeticC15_1_2,
                 TEST_ARITHMETIC_C15_1_2_EXPECT));

  printf("Tests Succeeded!\n");
}

static void testWriteChunk() {
  Chunk chunk;
  initChunk(&chunk);

  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);

  writeChunk(&chunk, OP_RETURN, 123);
  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

static void testArithmetic() {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  // Constant 1 = 1.2
  int constant = addConstant(&chunk, 1.2);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);

  // Constant 2 = 3.4
  constant = addConstant(&chunk, 3.4);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);

  // Pops Const 1 and Const 2, then pushes the sum of 4.6
  writeChunk(&chunk, OP_ADD, 123);

  // Constant 3 = 5.6
  constant = addConstant(&chunk, 5.6);
  writeChunk(&chunk, OP_CONSTANT, 123);
  writeChunk(&chunk, constant, 123);

  // (C1 + C2) / C3 = .821~ish
  writeChunk(&chunk, OP_DIVIDE, 123);

  // -((C1 + C2) / C3) = -.821~ish
  writeChunk(&chunk, OP_NEGATE, 123);

  writeChunk(&chunk, OP_RETURN, 123);
  assert(interpret(&chunk) == INTERPRET_OK);
}

#define ADD_CONSTANT(val, line)                                                \
  do {                                                                         \
    int constant = addConstant(&chunk, val);                                   \
    writeChunk(&chunk, OP_CONSTANT, line);                                     \
    writeChunk(&chunk, constant, line);                                        \
  } while (false)

static char TEST_ARITHMETIC_C15_1_1_EXPECT[] = "5\n";

static void testArithmeticC15_1_1() {
  // 1 * 2 + 3

  initVM();

  Chunk chunk;
  initChunk(&chunk);

  // 1 * 2
  ADD_CONSTANT(1, 123);
  ADD_CONSTANT(2, 123);
  writeChunk(&chunk, OP_MULTIPLY, 123);

  // 2 + 3
  ADD_CONSTANT(3, 123);
  writeChunk(&chunk, OP_ADD, 123);

  writeChunk(&chunk, OP_RETURN, 123);

  assert(interpret(&chunk) == INTERPRET_OK);
}

static char TEST_ARITHMETIC_C15_1_2_EXPECT[] = "7\n";

static void testArithmeticC15_1_2() {
  // 1 + 2 * 3

  initVM();

  Chunk chunk;
  initChunk(&chunk);

  // 1 * 2
  ADD_CONSTANT(1, 123);
  ADD_CONSTANT(2, 123);
  writeChunk(&chunk, OP_MULTIPLY, 123);

  // 2 + 3
  ADD_CONSTANT(3, 123);
  writeChunk(&chunk, OP_ADD, 123);

  writeChunk(&chunk, OP_RETURN, 123);

  assert(interpret(&chunk) == INTERPRET_OK);
}
#undef ADD_CONSTANT

/***
 * Takes in a functionPtr test case and checks if the stdout output is equal to
 * the expect string.
 *
 * @param testName The name of the test being run.
 * @param testCase A functionPtr which should print output (ideally) equal to
 * the expect param.
 * @param expect A string which is expected to equal the output of the testCase.
 *
 * @return Whether or not the test passed or failed.
 */
static bool runTest(char *testName, void (*testCase)(), char *expect) {
  char buf[1024];
  FILE *membufstdout = fmemopen(buf, sizeof(buf), "w");

  FILE *prev_stdout = stdout;
  stdout = membufstdout;

  testCase();

  long end_pos = ftell(membufstdout);
  fclose(stdout);

  stdout = prev_stdout;

  buf[end_pos] = '\0';
  char *result = buf;

  if (strcmp(result, expect)) {
    printf("Test '%s' failed.\nExpect: '%s'\nResult: '%s'\n", testName, expect,
           result);

    return false;
  }
  return true;
}
