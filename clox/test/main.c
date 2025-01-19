#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "chunk.h"
#include "debug.h"
#include "vm.h"

#define CONSTANT(val, line)                                                    \
  do {                                                                         \
    int constant = addConstant(&chunk, val);                                   \
    writeChunk(&chunk, OP_CONSTANT, line);                                     \
    writeChunk(&chunk, constant, line);                                        \
  } while (false)

static bool runTest(char *testName, void (*testCase)(), char *expect);

static char TEST_WRITE_CHUNK_EXPECT[];
static void testWriteChunk();

static char TEST_ARITHMETIC_EXPECT[];
static void testArithmetic();

static char TEST_ARITHMETIC_C15_2_EXPECT[];
static void testArithmetic_C15_2();

int main(int argc, char *argv[]) {
  printf("Starting Tests (:\n");

  assert(runTest("testWriteChunk", testWriteChunk, TEST_WRITE_CHUNK_EXPECT));
  assert(runTest("testArithmetic", testArithmetic, TEST_ARITHMETIC_EXPECT));
  assert(runTest("testArithmetic_C15_2", testArithmetic_C15_2,
                 TEST_ARITHMETIC_C15_2_EXPECT));

  printf("Tests Succeeded!\n");
}

static char TEST_WRITE_CHUNK_EXPECT[] =
    "== test chunk ==\n"
    "0000  123 OP_CONSTANT         0 '1.2'\n"
    "0002    | OP_RETURN\n";
static void testWriteChunk() {
  Chunk chunk;
  initChunk(&chunk);

  CONSTANT(1.2, 123);

  writeChunk(&chunk, OP_RETURN, 123);
  disassembleChunk(&chunk, "test chunk");
  freeChunk(&chunk);
}

static char TEST_ARITHMETIC_EXPECT[] = "-0.821429\n";
static void testArithmetic() {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  CONSTANT(1.2, 123);
  CONSTANT(3.4, 123);
  // Pops Const 1 and Const 2, then pushes the sum of 4.6
  writeChunk(&chunk, OP_ADD, 123);

  CONSTANT(5.6, 123);
  // (C1 + C2) / C3 = .821~ish
  writeChunk(&chunk, OP_DIVIDE, 123);
  // -((C1 + C2) / C3) = -.821~ish
  writeChunk(&chunk, OP_NEGATE, 123);
  writeChunk(&chunk, OP_RETURN, 123);
  assert(interpret(&chunk) == INTERPRET_OK);
}

static char TEST_ARITHMETIC_C15_2_EXPECT[] = "10\n10\n";
static void testArithmetic_C15_2() {
  initVM();

  Chunk chunk;
  initChunk(&chunk);

  // 4 - 3 * -2 - No Negate
  CONSTANT(4, 123);
  CONSTANT(3, 123);

  // To make -2
  CONSTANT(0, 123);
  CONSTANT(2, 123);
  writeChunk(&chunk, OP_SUBTRACT, 123);

  // 3 * -2
  writeChunk(&chunk, OP_MULTIPLY, 123);

  // 4 - -6 = 10
  writeChunk(&chunk, OP_SUBTRACT, 123);
  writeChunk(&chunk, OP_RETURN, 123);

  assert(interpret(&chunk) == INTERPRET_OK);
  // 4 - 3 * -2 - No Subtract
  CONSTANT(4, 123);
  CONSTANT(3, 123);

  CONSTANT(2, 123);
  writeChunk(&chunk, OP_NEGATE, 123);

  // 3 * -2
  writeChunk(&chunk, OP_MULTIPLY, 123);


  // 4 - -6 = 10
  writeChunk(&chunk, OP_NEGATE, 123);
  writeChunk(&chunk, OP_ADD, 123);
  writeChunk(&chunk, OP_RETURN, 123);
  assert(interpret(&chunk) == INTERPRET_OK);
}
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
