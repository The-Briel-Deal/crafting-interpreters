#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "helper_test.h"
#include "object.h"
#include "table.h"
#include "value.h"

#define ALLOCATE_OBJ(type, objectType)                                         \
  (type *)allocateObject(sizeof(type), objectType)

const char TEST_TABLE_1_EXPECT[] = "testVal";
void       testTable1() {
  Table table;
  initTable(&table);
  char       key[]  = "testKey";
  ObjString *keyObj = copyString(key, sizeof(key));
  char       val[]  = "testVal";
  ObjString *valObj = copyString(val, sizeof(val));

  bool isNewKey = tableSet(&table, keyObj, OBJ_VAL(valObj));
  assert(isNewKey == true);

  Value resultVal;
  bool  foundKey = tableGet(&table, keyObj, &resultVal);
  assert(foundKey == true);

  assert(resultVal.type == VAL_OBJ);
  Obj *resultObj = AS_OBJ(resultVal);

  assert(resultObj->type == OBJ_STRING);
  ObjString *resultObjString = (ObjString *)resultObj;

  assert(resultObjString->length == sizeof(val));

  assert(memcmp(resultObjString->chars, val, resultObjString->length) == 0);

	printf("%s", resultObjString->chars);
}

const TestCase TABLE_TESTS[] = {
    {"testTable1", testTable1, TEST_TABLE_1_EXPECT},
};

const int TABLE_TESTS_COUNT = sizeof(TABLE_TESTS) / sizeof(TestCase);
