
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
  for (Obj *object = vm.objects; object != NULL; object = object->next) {
    for (int i = 0; i < len; i++) {
      struct StrToFind *str = &strsToFind[i];
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
    printf("Test failed.");
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

void testSortObjsByAddr1() {
  Obj objects[] = {
      [0] = {.isMarked = true, .next = NULL, .type = OBJ_STRING},
      [1] = {.isMarked = true, .next = NULL, .type = OBJ_STRING},
      [2] = {.isMarked = true, .next = NULL, .type = OBJ_STRING},
      [3] = {.isMarked = true, .next = NULL, .type = OBJ_STRING},
  };

  // 2 -> 0 -> 1 -> 3 ->
  objects[2].next = &objects[0];
  objects[0].next = &objects[1];
  objects[1].next = &objects[3];
  objects[3].next = NULL;

  Obj *head = &objects[2];

  head = sortObjsByAddr(head);

  assert(head == &objects[0]);
  assert(head->next == &objects[1]);
  assert(head->next->next == &objects[2]);
  assert(head->next->next->next == &objects[3]);
  assert(head->next->next->next->next == NULL);
}

void testSortObjsByAddr2() {
  Obj objects[] = {
      [0] = {.isMarked = true, .next = NULL, .type = OBJ_STRING},
      [1] = {.isMarked = true, .next = NULL, .type = OBJ_STRING},
      [2] = {.isMarked = true, .next = NULL, .type = OBJ_STRING},
      [3] = {.isMarked = true, .next = NULL, .type = OBJ_STRING},
      [4] = {.isMarked = true, .next = NULL, .type = OBJ_STRING},
      [5] = {.isMarked = true, .next = NULL, .type = OBJ_STRING},
  };

  // 5 -> 4 -> 3 -> 2 -> 1 -> 0
  objects[5].next = &objects[4];
  objects[4].next = &objects[3];
  objects[3].next = &objects[2];
  objects[2].next = &objects[1];
  objects[1].next = &objects[0];
  objects[0].next = NULL;

  Obj *head = &objects[5];

  head = sortObjsByAddr(head);

  assert(head == &objects[0]);
  assert(head->next == &objects[1]);
  assert(head->next->next == &objects[2]);
  assert(head->next->next->next == &objects[3]);
  assert(head->next->next->next->next == &objects[4]);
  assert(head->next->next->next->next->next == &objects[5]);
  assert(head->next->next->next->next->next->next == NULL);
}

void testGC() {
  printf("Running Garbage Collector Tests:\n");
  // TODO: Re-enable these tests once GC is implemented.
  //  printf("Running Test: 'testStrsOnStack'\n");
  //  testStrsOnStack();
  //  printf("Running Test: 'testManyStrsOnStack'\n");
  //  testManyStrsOnStack();
  printf("Running Test: 'testSortObjsByAddr1'\n");
  testSortObjsByAddr1();
  printf("Running Test: 'testSortObjsByAddr2'\n");
  testSortObjsByAddr2();
}
