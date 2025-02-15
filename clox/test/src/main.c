#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "chunk_test.h"
#include "helper_test.h"
#include "line_test.h"
#include "scanner_test.h"
#include "table_test.h"
#include "vm_test.h"

#define RUN_TESTS(TESTS, TEST_COUNT)                                           \
  for (int i = 0; i < TEST_COUNT; i++) {                                       \
    TestCase test = TESTS[i];                                                  \
    assert(runTest(test.name, test.fn, test.expect));                          \
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

int main(int argc, char *argv[]) {
  printf("Starting Tests (:\n");

  RUN_TESTS(CHUNK_TESTS, CHUNK_TESTS_COUNT);
  RUN_TESTS(VM_TESTS, VM_TESTS_COUNT);
  RUN_TESTS(SCANNER_TESTS, SCANNER_TESTS_COUNT);
  RUN_TESTS(TABLE_TESTS, TABLE_TESTS_COUNT);
  RUN_TESTS(LINE_TESTS, LINE_TESTS_COUNT);

  printf("Tests Succeeded!\n");
}
