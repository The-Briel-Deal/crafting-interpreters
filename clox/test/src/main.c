#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "chunk_test.h"
#include "helper_test.h"
#include "scanner_test.h"
#include "vm_test.h"

int main(int argc, char *argv[]) {
  printf("Starting Tests (:\n");

  assert(runTest("testWriteChunk", testWriteChunk, TEST_WRITE_CHUNK_EXPECT));
  assert(runTest("testArithmetic", testArithmetic, TEST_ARITHMETIC_EXPECT));
  assert(runTest("testScanner", testScanner, TEST_SCANNER_EXPECT));

  printf("Tests Succeeded!\n");
}
