#include <assert.h>
#include <stdbool.h>

#include "helper_test.h"

void testVal1() {
  assert(false);
}

const TestCase VALUE_TESTS[] = {
    {"testVal1", testVal1, ""},
};

const int VALUE_TESTS_COUNT = sizeof(VALUE_TESTS) / sizeof(TestCase);
