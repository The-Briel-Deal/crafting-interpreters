#ifndef clox_value_h
#define clox_value_h

#include <stdbool.h>

typedef enum { VAL_BOOL, VAL_NIL, VAL_NUMBER } ValueType;

typedef struct {
  ValueType type;
  union {
    bool   boolean;
    double number;
  } as;
} Value;

#define BOOL_VAL(value)   ((Value){VAL_BOOL, {.boolean = value}})
#define NIL_VAL           ((Value){VAL_NIL, {.number = 0}})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, {.number = value}})

typedef struct {
  int    capacity;
  int    count;
  Value *values;
} ValueArray;

void initValueArray(ValueArray *array);
void writeValueArray(ValueArray *array, Value value);
void freeValueArray(ValueArray *array);
void printValue(Value value);

#endif
