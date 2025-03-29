#ifndef clox_value_h
#define clox_value_h

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct Obj Obj;
typedef struct ObjString ObjString;

#ifdef NAN_BOXING

#define SIGN_BIT  ((uint64_t)0x8000000000000000)
#define QNAN      ((uint64_t)0x7ffc000000000000)

#define TAG_NIL   1
#define TAG_FALSE 2
#define TAG_TRUE  3

typedef uint64_t Value;

#define IS_BOOL(value)   (((value) | 1) == TRUE_VAL)
#define IS_NIL(value)    ((value) == NIL_VAL)
#define IS_NUMBER(value) (((value) & QNAN) != QNAN)
#define IS_OBJ(value)    (((value) & (QNAN | SIGN_BIT)) == (QNAN | SIGN_BIT))

#define AS_BOOL(value)   ((value) == TRUE_VAL)
#define AS_NUMBER(value) valueToNum(value)
#define AS_OBJ(value)    ((Obj *)(uintptr_t)((value) & ~(SIGN_BIT | QNAN)))

#define BOOL_VAL(b)      ((b) ? TRUE_VAL : FALSE_VAL)
#define FALSE_VAL        ((Value)(uint64_t)(QNAN | TAG_FALSE))
#define TRUE_VAL         ((Value)(uint64_t)(QNAN | TAG_TRUE))
#define NIL_VAL          ((Value)(uint64_t)(QNAN | TAG_NIL))
#define NUMBER_VAL(num)  numToValue(num)
#define OBJ_VAL(obj)     (Value)(SIGN_BIT | QNAN | (uint64_t)(uintptr_t)(obj))

// These should optimize the memcpy away.
static inline double valueToNum(Value value) {
  double num;
  memcpy(&num, &value, sizeof(value));
  return num;
}
static inline Value numToValue(double num) {
  Value value;
  memcpy(&value, &num, sizeof(double));
  return value;
}

#else

typedef enum {
  VAL_BOOL,
  VAL_NIL,
  VAL_NUMBER,
  VAL_SMALL_STR,
  VAL_OBJ
} ValueType;

typedef struct {
  uint8_t len;
  char start[7];
} SmallStr;

static inline SmallStr newSmallStr(uint8_t len, char *start) {
  assert(len <= 7);
  SmallStr smallStr = {.len = len};
  strncpy(smallStr.start, start, len);
  return smallStr;
}

typedef struct {
  ValueType type;
  union {
    bool boolean;
    double number;
    Obj *obj;
    SmallStr smallStr;
  } as;
} Value;

#define IS_BOOL(value)          ((value).type == VAL_BOOL)
#define IS_NIL(value)           ((value).type == VAL_NIL)
#define IS_NUMBER(value)        ((value).type == VAL_NUMBER)
#define IS_OBJ(value)           ((value).type == VAL_OBJ)
#define IS_SMALL_STR(value)     ((value).type == VAL_SMALL_STR)

#define AS_OBJ(value)           ((value).as.obj)
#define AS_BOOL(value)          ((value).as.boolean)
#define AS_NUMBER(value)        ((value).as.number)
#define AS_SMALL_STR(value)     ((value).as.smallStr)

#define BOOL_VAL(value)         ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL                 ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value)       ((Value){VAL_NUMBER, {.number = value}})
#define OBJ_VAL(object)         ((Value){VAL_OBJ, {.obj = (Obj *)object}})
#define SMALL_STR_VAL(smallStr) ((Value){VAL_SMALL_STR, {.smallStr = smallStr}})

#endif

typedef struct {
  int capacity;
  int count;
  Value *values;
} ValueArray;

bool valuesEqual(Value a, Value b);
void initValueArray(ValueArray *array);
void writeValueArray(ValueArray *array, Value value);
void freeValueArray(ValueArray *array);
void printValue(Value value);

#endif
