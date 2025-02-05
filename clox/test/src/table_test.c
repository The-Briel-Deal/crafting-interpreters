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
  Value key;
  char *val;
};

#define COPY_LITERAL(str) copyString(str, sizeof(str))

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
                                   "Googieeee -> Boingleeee\n"
																	 "2.0 -> Boogie\n";

void testTable2() {

  const struct KeyValPair TEST_TABLE_2_KEY_VAL_PAIRS[] = {
      {.key = OBJ_VAL(COPY_LITERAL("Boogie")),       .val = "Woogie"    },
      {.key = OBJ_VAL(COPY_LITERAL("Schmoogie")),    .val = "Doogie"    },
      {.key = OBJ_VAL(COPY_LITERAL("Googie")),       .val = "Boingle"   },
      {.key = OBJ_VAL(COPY_LITERAL("Boogiee")),      .val = "Woogiee"   },
      {.key = OBJ_VAL(COPY_LITERAL("Schmoogiee")),   .val = "Doogiee"   },
      {.key = OBJ_VAL(COPY_LITERAL("Googiee")),      .val = "Boinglee"  },
      {.key = OBJ_VAL(COPY_LITERAL("Boogieee")),     .val = "Woogieee"  },
      {.key = OBJ_VAL(COPY_LITERAL("Schmoogieee")),  .val = "Doogieee"  },
      {.key = OBJ_VAL(COPY_LITERAL("Googieee")),     .val = "Boingleee" },
      {.key = OBJ_VAL(COPY_LITERAL("Boogieeee")),    .val = "Woogieeee" },
      {.key = OBJ_VAL(COPY_LITERAL("Schmoogieeee")), .val = "Doogieeee" },
      {.key = OBJ_VAL(COPY_LITERAL("Googieeee")),    .val = "Boingleeee"},
      {.key = NUMBER_VAL(2),                         .val = "Boogie"    },
  };
  Table table;
  initTable(&table);

  // Set Key-Val Pairs
  for (int i = 0;
       i < (sizeof(TEST_TABLE_2_KEY_VAL_PAIRS) / sizeof(struct KeyValPair));
       i++) {
    Value      key    = TEST_TABLE_2_KEY_VAL_PAIRS[i].key;
    char      *val    = TEST_TABLE_2_KEY_VAL_PAIRS[i].val;
    ObjString *valObj = copyString(val, strlen(val));

    bool isNewKey = tableSet(&table, key, OBJ_VAL(valObj));
    assert(isNewKey == true);
  }

  // Get Vals
  for (int i = 0;
       i < (sizeof(TEST_TABLE_2_KEY_VAL_PAIRS) / sizeof(struct KeyValPair));
       i++) {
    Value key = TEST_TABLE_2_KEY_VAL_PAIRS[i].key;
    char *val = TEST_TABLE_2_KEY_VAL_PAIRS[i].val;

    Value resultVal;
    bool  foundKey = tableGet(&table, key, &resultVal);
    assert(foundKey == true);

    assert(resultVal.type == VAL_OBJ);
    Obj *resultObj = AS_OBJ(resultVal);

    assert(resultObj->type == OBJ_STRING);
    ObjString *resultObjString = (ObjString *)resultObj;

    assert(resultObjString->length == strlen(val));

    assert(memcmp(resultObjString->chars, val, resultObjString->length) == 0);

    if (IS_STRING(key)) {
      printf("%s -> %s\n", AS_CSTRING(key), resultObjString->chars);
    } else if (IS_NUMBER(key)) {
      printf("%.1f -> %s\n", AS_NUMBER(key), resultObjString->chars);
    }
  }
}

const TestCase TABLE_TESTS[] = {
    {"testTable1", testTable1, TEST_TABLE_1_EXPECT},
    {"testTable2", testTable2, TEST_TABLE_2_EXPECT},
};

const int TABLE_TESTS_COUNT = sizeof(TABLE_TESTS) / sizeof(TestCase);
