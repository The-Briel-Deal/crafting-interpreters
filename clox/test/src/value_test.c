#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "helper_test.h"
#include "object.h"
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

void testSmallStrToObj() {
  char testStr[]      = "foo";
  uint8_t testStrSize = sizeof(testStr);
  SmallStr smallStr   = newSmallStr(testStrSize, testStr);
  assert(testStrSize == smallStr.len);
  assert(memcmp(testStr, smallStr.start, testStrSize) == 0);

  // Convert to value.
  Value value = SMALL_STR_VAL(smallStr);
  assert(IS_SMALL_STR(value));

  // Convert to ObjString value.
  Value valObjString = smallStrToObjString(value);
  assert(IS_STRING(valObjString));

  // Validate valObjString.
  ObjString *objString = AS_STRING(valObjString);
  assert(testStrSize == objString->length);
  assert(memcmp(testStr, objString->chars, testStrSize) == 0);
}

const TestCase VALUE_TESTS[] = {
    {"testMakeSmallStr",    testMakeSmallStr,    ""},
    {"testMakeSmallStrVal", testMakeSmallStrVal, ""},
    {"testSmallStrToObj",   testSmallStrToObj,   ""},
};

const int VALUE_TESTS_COUNT = sizeof(VALUE_TESTS) / sizeof(TestCase);
