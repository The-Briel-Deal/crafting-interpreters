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

int main(int argc, char *argv[]) {
  printf("Starting Tests (:\n");

  char testWriteChunkExpect[] = "== test chunk ==\n"
                                "0000  123 OP_CONSTANT         0 '1.2'\n"
                                "0002    | OP_RETURN\n";
  assert(runTest("testWriteChunk", testWriteChunk, testWriteChunkExpect));

	char testArithmeticExpect[] = "-0.821429\n";
  assert(runTest("testArithmetic", testArithmetic, testArithmeticExpect));

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
    printf("Test '%s' failed.\nExpect: '%s'\nResult: '%s'", testName, expect,
           result);

    return false;
  }
  return true;
}
