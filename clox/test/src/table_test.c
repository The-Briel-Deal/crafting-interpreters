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
  int        key      = 1;
  Value      keyValue = NUMBER_VAL(key);
  char       val[]    = "testVal";
  ObjString *valObj   = copyString(val, sizeof(val));

  bool isNewKey = tableSet(&table, keyValue, OBJ_VAL(valObj));
  assert(isNewKey == true);

  Value resultVal;
  bool  foundKey = tableGet(&table, keyValue, &resultVal);
  assert(foundKey == true);

  assert(resultVal.type == VAL_OBJ);
  Obj *resultObj = AS_OBJ(resultVal);

  assert(resultObj->type == OBJ_STRING);
  ObjString *resultObjString = (ObjString *)resultObj;

  assert(resultObjString->length == sizeof(val));

  assert(memcmp(resultObjString->chars, val, resultObjString->length) == 0);

  printf("%s", resultObjString->chars);
}

struct KeyValPair {
  char *key;
  char *val;
};

const struct KeyValPair TEST_TABLE_2_KEY_VAL_PAIRS[] = {
    {.key = "Boogie",       .val = "Woogie"    },
    {.key = "Schmoogie",    .val = "Doogie"    },
    {.key = "Googie",       .val = "Boingle"   },
    {.key = "Boogiee",      .val = "Woogiee"   },
    {.key = "Schmoogiee",   .val = "Doogiee"   },
    {.key = "Googiee",      .val = "Boinglee"  },
    {.key = "Boogieee",     .val = "Woogieee"  },
    {.key = "Schmoogieee",  .val = "Doogieee"  },
    {.key = "Googieee",     .val = "Boingleee" },
    {.key = "Boogieeee",    .val = "Woogieeee" },
    {.key = "Schmoogieeee", .val = "Doogieeee" },
    {.key = "Googieeee",    .val = "Boingleeee"},
};

const char TEST_TABLE_2_EXPECT[] = "Boogie -> Woogie\n"
                                   "Schmoogie -> Doogie\n"
                                   "Googie -> Boingle\n"
                                   "Boogiee -> Woogiee\n"
                                   "Schmoogiee -> Doogiee\n"
                                   "Googiee -> Boinglee\n"
                                   "Boogieee -> Woogieee\n"
                                   "Schmoogieee -> Doogieee\n"
                                   "Googieee -> Boingleee\n"
                                   "Boogieeee -> Woogieeee\n"
                                   "Schmoogieeee -> Doogieeee\n"
                                   "Googieeee -> Boingleeee\n";

void testTable2() {
  Table table;
  initTable(&table);

  // Set Key-Val Pairs
  for (int i = 0;
       i < (sizeof(TEST_TABLE_2_KEY_VAL_PAIRS) / sizeof(struct KeyValPair));
       i++) {
    char      *key    = TEST_TABLE_2_KEY_VAL_PAIRS[i].key;
    ObjString *keyObj = copyString(key, strlen(key));
    char      *val    = TEST_TABLE_2_KEY_VAL_PAIRS[i].val;
    ObjString *valObj = copyString(val, strlen(val));

    bool isNewKey = tableSet(&table, OBJ_VAL(keyObj), OBJ_VAL(valObj));
    assert(isNewKey == true);
  }

  // Get Vals
  for (int i = 0;
       i < (sizeof(TEST_TABLE_2_KEY_VAL_PAIRS) / sizeof(struct KeyValPair));
       i++) {
    char      *key    = TEST_TABLE_2_KEY_VAL_PAIRS[i].key;
    ObjString *keyObj = copyString(key, strlen(key));
    char      *val    = TEST_TABLE_2_KEY_VAL_PAIRS[i].val;

    Value resultVal;
    bool  foundKey = tableGet(&table, OBJ_VAL(keyObj), &resultVal);
    assert(foundKey == true);

    assert(resultVal.type == VAL_OBJ);
    Obj *resultObj = AS_OBJ(resultVal);

    assert(resultObj->type == OBJ_STRING);
    ObjString *resultObjString = (ObjString *)resultObj;

    assert(resultObjString->length == strlen(val));

    assert(memcmp(resultObjString->chars, val, resultObjString->length) == 0);

    printf("%s -> %s\n", key, resultObjString->chars);
  }
}

const TestCase TABLE_TESTS[] = {
    {"testTable1", testTable1, TEST_TABLE_1_EXPECT},
    {"testTable2", testTable2, TEST_TABLE_2_EXPECT},
};

const int TABLE_TESTS_COUNT = sizeof(TABLE_TESTS) / sizeof(TestCase);
