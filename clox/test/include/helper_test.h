#ifndef clox_helper_test_h
#define clox_helper_test_h

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define CONSTANT(val, line)                                                    \
  do {                                                                         \
    int constant = addConstant(&chunk, val);                                   \
    writeChunk(&chunk, OP_CONSTANT, line);                                     \
    writeChunk(&chunk, constant, line);                                        \
  } while (false)

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

#endif
