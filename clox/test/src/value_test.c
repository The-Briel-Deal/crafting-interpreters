#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "helper_test.h"
#include "value.h"
#include "value_test.h"

void testMakeSmallStr() {
  char testStr[]      = "foo";
  uint8_t testStrSize = sizeof(testStr);
  SmallStr smallStr   = newSmallStr(testStrSize, testStr);
  assert(testStrSize == smallStr.len);
  assert(memcmp(testStr, smallStr.start, testStrSize) == 0);
}

void testMakeSmallStrVal() {
  char testStr[]      = "foo";
  uint8_t testStrSize = sizeof(testStr);
  SmallStr smallStr   = newSmallStr(testStrSize, testStr);
  assert(testStrSize == smallStr.len);
  assert(memcmp(testStr, smallStr.start, testStrSize) == 0);

  // Convert to value.
  Value value = SMALL_STR_VAL(smallStr);
  assert(IS_SMALL_STR(value));

  // Back to smallStr.
  SmallStr smallStrFromVal = AS_SMALL_STR(value);
  assert(testStrSize == smallStrFromVal.len);
  assert(memcmp(testStr, smallStrFromVal.start, testStrSize) == 0);
}

const TestCase VALUE_TESTS[] = {
    {"testMakeSmallStr",    testMakeSmallStr,    ""},
    {"testMakeSmallStrVal", testMakeSmallStrVal, ""},
};

const int VALUE_TESTS_COUNT = sizeof(VALUE_TESTS) / sizeof(TestCase);
