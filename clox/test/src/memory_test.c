
#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StrToFind {
  bool found;
  bool expectFound;
  char *str;
  int len;
};

void assertStrsOnHeap(struct StrToFind strsToFind[], int len) {
  // The only object on the heap should be testStr1.
  for (Obj *object = vm.heap.heapStart; object < (Obj *)(vm.heap.nextFree);
       object      = (Obj *)((char *)(object) + getObjSize(object->type))) {
    for (int i = 0; i < len; i++) {
      struct StrToFind *str = &strsToFind[i];

			//! For Debug:
      // if (object->type == OBJ_STRING) {
      //   printf("On Heap: %s\nExpect: %s\n", ((ObjString *)object)->chars,
      //          str->str);
      // }

      if (object->type == OBJ_STRING &&
          ((ObjString *)object)->length == str->len &&
          memcmp(((ObjString *)object)->chars, str->str, str->len) == 0) {
        str->found = true;
      }
    }
  }

  bool strNotFound = false;
  for (int i = 0; i < len; i++) {
    if (strsToFind[i].found != strsToFind[i].expectFound) {
      printf("String '%s' found field did not match expectation.\n",
             strsToFind[i].str);
      strNotFound = true;
    }
  }
  if (strNotFound) {
    printf("Test failed.\n");
    exit(1);
  }
}

void testStrsOnStack() {
  initVM();
  char testStr1[]        = "testStr1";
  ObjString *testStr1Obj = copyString(testStr1, sizeof(testStr1));

  char testStr2[] = "testStr2";
  copyString(testStr2, sizeof(testStr2));
  push(OBJ_VAL(testStr1Obj));

  collectGarbage();

  struct StrToFind strsToFind[] = {
      [0] = {.str         = testStr1,
             .len         = sizeof(testStr1),
             .found       = false,
             .expectFound = true },
      [1] = {.str         = testStr2,
             .len         = sizeof(testStr2),
             .found       = false,
             .expectFound = false}
  };
  assertStrsOnHeap(strsToFind, sizeof(strsToFind) / sizeof(struct StrToFind));
}

void testManyStrsOnStack() {
  initVM();
  char testStr1[]        = "testStr1";
  ObjString *testStr1Obj = copyString(testStr1, sizeof(testStr1));

  char testStr2[] = "testStr2";
  copyString(testStr2, sizeof(testStr2));

  push(OBJ_VAL(testStr1Obj));

  char testStr3[]        = "testStr3";
  ObjString *testStr3Obj = copyString(testStr3, sizeof(testStr3));
  push(OBJ_VAL(testStr3Obj));

  char testStr4[]        = "testStr4";
  ObjString *testStr4Obj = copyString(testStr4, sizeof(testStr4));
  push(OBJ_VAL(testStr4Obj));

  char testStr5[]        = "testStr5";
  ObjString *testStr5Obj = copyString(testStr5, sizeof(testStr5));
  push(OBJ_VAL(testStr5Obj));

  collectGarbage();

  struct StrToFind strsToFind[] = {
      [0] = {.str         = testStr1,
             .len         = sizeof(testStr1),
             .found       = false,
             .expectFound = true },
      [1] = {.str         = testStr2,
             .len         = sizeof(testStr2),
             .found       = false,
             .expectFound = false},
      [2] = {.str         = testStr3,
             .len         = sizeof(testStr3),
             .found       = false,
             .expectFound = true },
      [3] = {.str         = testStr4,
             .len         = sizeof(testStr4),
             .found       = false,
             .expectFound = true },
      [4] = {.str         = testStr5,
             .len         = sizeof(testStr5),
             .found       = false,
             .expectFound = true },
  };

  assertStrsOnHeap(strsToFind, sizeof(strsToFind) / sizeof(struct StrToFind));
}

void testGC() {
  printf("Running Garbage Collector Tests:\n");
  printf("Running Test: 'testStrsOnStack'\n");
  testStrsOnStack();
  printf("Running Test: 'testManyStrsOnStack'\n");
  testManyStrsOnStack();
}
