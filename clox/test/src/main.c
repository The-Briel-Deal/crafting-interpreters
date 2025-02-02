#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "chunk.h"
#include "debug.h"

#include "helper_test.h"
#include "scanner_test.h"
#include "vm_test.h"

static bool runTest(char *testName, void (*testCase)(), const char *expect);

static char TEST_WRITE_CHUNK_EXPECT[];
static void testWriteChunk();

int main(int argc, char *argv[]) {
  printf("Starting Tests (:\n");

  assert(runTest("testWriteChunk", testWriteChunk, TEST_WRITE_CHUNK_EXPECT));
  assert(runTest("testArithmetic", testArithmetic, TEST_ARITHMETIC_EXPECT));
  assert(runTest("testScanner", testScanner, TEST_SCANNER_EXPECT));

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
static bool runTest(char *testName, void (*testCase)(), const char *expect) {
  char buf[1024];
  FILE *membufstdout = fmemopen(buf, sizeof(buf), "w");

  FILE *prev_stdout = stdout;
  stdout            = membufstdout;

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
