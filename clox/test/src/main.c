#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "chunk_test.h"
#include "helper_test.h"
#include "scanner_test.h"
#include "table_test.h"
#include "vm_test.h"

#define RUN_TESTS(TESTS, TEST_COUNT)                                           \
  for (int i = 0; i < TEST_COUNT; i++) {                                       \
    TestCase test = TESTS[i];                                                  \
    assert(runTest(test.name, test.fn, test.expect));                          \
  }

int main(int argc, char *argv[]) {
  printf("Starting Tests (:\n");

  RUN_TESTS(CHUNK_TESTS, CHUNK_TESTS_COUNT);
  RUN_TESTS(VM_TESTS, VM_TESTS_COUNT);
  RUN_TESTS(SCANNER_TESTS, SCANNER_TESTS_COUNT);
  RUN_TESTS(TABLE_TESTS, TABLE_TESTS_COUNT);

  printf("Tests Succeeded!\n");
}
