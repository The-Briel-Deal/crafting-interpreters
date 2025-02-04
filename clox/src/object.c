#include <stdio.h>
#include <string.h>

#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType)                                         \
  (type *)allocateObject(sizeof(type), objectType)

static Obj *allocateObject(size_t size, ObjType type) {
  Obj *object  = (Obj *)reallocate(NULL, 0, size);
  object->type = type;

  object->next = vm.objects;
  vm.objects   = object;
  return object;
}

static ObjString *allocateString(char *chars, int length, bool constant) {
  ObjString *string      = ALLOCATE_OBJ(ObjString, OBJ_STRING);
  string->length         = length;
  string->chars          = chars;
  string->constantString = constant;
  return string;
}

ObjString *takeString(char *chars, int length) {
  return allocateString(chars, length, false);
}

ObjString *copyString(char *chars, int length) {
  return allocateString(chars, length, true);
}

void printObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case OBJ_STRING: printf("%s", AS_CSTRING(value)); break;
  }
}
