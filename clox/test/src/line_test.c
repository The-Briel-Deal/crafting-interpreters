#include <assert.h>
#include <string.h>

#include "helper_test.h"
#include "line.h"
#include "line_test.h"

#define ALLOCATE_OBJ(type, objectType)                                         \
  (type *)allocateObject(sizeof(type), objectType)

const char TEST_LINE_1_EXPECT[] = "";
void testLine1() {
  struct Line line;
  initLine(&line);
  insertLine(&line, 'a');
  assert(line.length == 1);
  assert(line.pos == 1);
  assert(memcmp(line.start, "a", line.length) == 0);

  insertLine(&line, 'b');
  assert(line.length == 2);
  assert(line.pos == 2);
  assert(memcmp(line.start, "ab", line.length) == 0);

  insertLine(&line, 'c');
  assert(line.length == 3);
  assert(line.pos == 3);
  assert(memcmp(line.start, "abc", line.length) == 0);

  setPosLine(&line, 1);
  assert(line.pos == 1);

  insertLine(&line, '1');
  assert(line.length == 4);
  assert(line.pos == 2);
  assert(memcmp(line.start, "a1bc", line.length) == 0);

  insertLine(&line, '2');
  assert(line.length == 5);
  assert(line.pos == 3);
  assert(memcmp(line.start, "a12bc", line.length) == 0);

  setPosLine(&line, 4);

  removeLine(&line);
  assert(line.length == 4);
  assert(line.pos == 3);
  assert(memcmp(line.start, "a12c", line.length) == 0);

  // Nothing should change if I try to delete at beginning.
  setPosLine(&line, 0);

  removeLine(&line);
  assert(line.length == 4);
  assert(line.pos == 0);
  assert(memcmp(line.start, "a12c", line.length) == 0);
}

const TestCase LINE_TESTS[] = {
    {"testLine1", testLine1, TEST_LINE_1_EXPECT},
};

const int LINE_TESTS_COUNT = sizeof(LINE_TESTS) / sizeof(TestCase);
