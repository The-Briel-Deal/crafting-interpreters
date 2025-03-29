#include <assert.h>
#include <stdio.h>

#include "memory.h"
#include "object.h"
#include "value.h"

void initValueArray(ValueArray *array) {
  array->values   = NULL;
  array->capacity = 0;
  array->count    = 0;
}

void writeValueArray(ValueArray *array, Value value) {
  if (array->capacity < array->count + 1) {
    int oldCapacity = array->capacity;
    array->capacity = GROW_CAPACITY(oldCapacity);
    array->values =
        GROW_ARRAY(Value, array->values, oldCapacity, array->capacity);
  }

  array->values[array->count] = value;
  array->count++;
}

void freeValueArray(ValueArray *array) {
  FREE_ARRAY(Value, array->values, array->capacity);
  initValueArray(array);
}

void printValue(Value value) {
#ifdef NAN_BOXING
  if (IS_BOOL(value)) {
    printf(AS_BOOL(value) ? "true" : "false");
  } else if (IS_NIL(value)) {
    printf("nil");
  } else if (IS_NUMBER(value)) {
    printf("%g", AS_NUMBER(value));
  } else if (IS_OBJ(value)) {
    printObject(value);
  }
#else
  switch (value.type) {
    case VAL_BOOL  : printf(AS_BOOL(value) ? "true" : "false"); break;
    case VAL_NIL   : printf("nil"); break;
    case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
    case VAL_OBJ   : printObject(value); break;
    case VAL_SMALL_STR:
      printf("%.*s", AS_SMALL_STR(value).len, AS_SMALL_STR(value).start);
      break;
    default: assert(false); // Should be unreachable.
  }
#endif
}

bool valuesEqual(Value a, Value b) {
#ifdef NAN_BOXING
  if (IS_NUMBER(a) && IS_NUMBER(b)) {
    return AS_NUMBER(a) == AS_NUMBER(b);
  }
  return a == b;
#else
  if (a.type != b.type)
    return false;
  switch (a.type) {
    case VAL_BOOL  : return AS_BOOL(a) == AS_BOOL(b);
    case VAL_NIL   : return true;
    case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
    case VAL_OBJ   : return AS_OBJ(a) == AS_OBJ(b);
    default        : assert(false); return false; // Should be unreachable.
  }
#endif
}

Value smallStrToObjString(Value smallStrVal) {
  assert(IS_SMALL_STR(smallStrVal));
  SmallStr smallStr    = AS_SMALL_STR(smallStrVal);
  ObjString *objString = copyString(smallStr.start, smallStr.len);
  return OBJ_VAL(objString);
}

char *getStringChars(Value *value) {
  if (IS_STRING(*value)) {
    return AS_STRING(*value)->chars;

  } else if (IS_SMALL_STR(*value)) {
    return AS_SMALL_STR(*value).start;
  }
  assert(false);
}

int getStringLen(Value *value) {
  if (IS_STRING(*value)) {
    return AS_STRING(*value)->length;

  } else if (IS_SMALL_STR(*value)) {
    return AS_SMALL_STR(*value).len;
  }
  assert(false);
}
