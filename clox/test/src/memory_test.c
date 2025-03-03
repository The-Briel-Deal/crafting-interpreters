
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
  char *str;
  int len;
};

void testStrsOnStack() {
  initVM();
  char testStr1[]        = "testStr1";
  ObjString *testStr1Obj = copyString(testStr1, sizeof(testStr1));

  char testStr2[] = "testStr2";
  // ObjString *testStr2Obj =
  copyString(testStr2, sizeof(testStr2));
  push(OBJ_VAL(testStr1Obj));

  collectGarbage();

  struct StrToFind strsToFind[] = {
      [0] = {.str = testStr1, .len = sizeof(testStr1), .found = false}
  };
  // The only object on the heap should be testStr1.
  for (Obj *object = vm.objects; object != NULL; object = object->next) {
    for (int i = 0; i < sizeof(strsToFind) / sizeof(struct StrToFind); i++) {
      struct StrToFind *str = &strsToFind[i];
      if (object->type == OBJ_STRING &&
          ((ObjString *)object)->length == str->len &&
          memcmp(((ObjString *)object)->chars, str->str, str->len) == 0) {
        str->found = true;
      }
    }
  }

  bool strNotFound = false;
  for (int i = 0; i < sizeof(strsToFind) / sizeof(struct StrToFind); i++) {
    if (strsToFind[i].found == false) {
      printf("String '%s' not found on heap.", strsToFind[i].str);
      strNotFound = true;
    }
  }
  if (strNotFound) {
    printf("Test failed.");
    exit(1);
  }
}

void testGC() {
  printf("Running Garbage Collector Tests:\n");
  testStrsOnStack();
}
