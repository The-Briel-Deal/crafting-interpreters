#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "helper_test.h"
#include "value.h"
#include "value_test.h"

void testMakeSmallStr() {
  char testStr[]      = "foo";
  uint8_t testStrSize = sizeof(testStr);
  SmallStr smallStr = newSmallStr(testStrSize, testStr);
	assert(memcmp(testStr, smallStr.start, testStrSize) == 0);
}

const TestCase VALUE_TESTS[] = {
    {"testMakeSmallStr", testMakeSmallStr, ""},
};

const int VALUE_TESTS_COUNT = sizeof(VALUE_TESTS) / sizeof(TestCase);
