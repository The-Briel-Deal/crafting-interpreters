#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chunk.h"
#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#define ALLOCATE_OBJ(type, objectType)                                         \
  (type *)allocateObject(sizeof(type), objectType)

#define HEAP_SIZE (1024 * 1024)

struct ObjHeap heap = {
    .start = NULL,
    .next  = NULL,
    .size  = 0,
};

void initHeap() {
  heap.start = malloc(HEAP_SIZE);
  heap.next  = NULL;
  heap.size  = HEAP_SIZE;
}
static void *allocateOnHeap(size_t size) {
  if (heap.start == NULL) {
    printf("Heap Not Initialized, Exiting.");
    exit(1);
  }

  // This means there are no items on the heap.
  if (heap.next == NULL) {
    heap.next = heap.start;
  }

  void *ptr = heap.next;
  heap.next = ((uint8_t *)heap.next) + size;
  return ptr;
}

static int getObjListLength(Obj *objects) {
  int count = 0;
  while (objects != NULL) {
    count++;
    objects = objects->next;
  }
  return count;
}

// Returns new head.
Obj *sortObjsByAddr(Obj *objects) {
  Obj *head  = objects;
  int length = getObjListLength(objects);

  for (int i = 0; i < length; i++) {
    Obj *prev    = NULL;
    Obj *curr    = head;
    bool swapped = false;
    while (curr->next != NULL) {
      Obj *next = curr->next;
      // Swap
      if (curr > next) {
        swapped    = true;
        curr->next = next->next;
        next->next = curr;
        if (prev != NULL) {
          prev->next = next;
        } else {
          head = next;
        }
      }
      prev = curr;
      curr = curr->next;
    }
    // If we walk through list without swapping then everything is sorted.
    if (!swapped)
      break;
  }

  return head;
}

// void compactHeap() {
// }

static Obj *allocateObject(size_t size, ObjType type) {
  Obj *object      = (Obj *)allocateOnHeap(size);
  object->type     = type;
  object->isMarked = false;

  object->next = vm.objects;
  vm.objects   = object;

#ifdef DEBUG_LOG_GC
  printf("%p allocate %zu for %d\n", (void *)object, size, type);
#endif

  return object;
}

ObjClosure *newClosure(ObjFunction *function) {
  ObjUpvalue **upvalues = ALLOCATE(ObjUpvalue *, function->upvalueCount);
  for (int i = 0; i < function->upvalueCount; i++) {
    upvalues[i] = NULL;
  }

  ObjClosure *closure   = ALLOCATE_OBJ(ObjClosure, OBJ_CLOSURE);
  closure->function     = function;
  closure->upvalues     = upvalues;
  closure->upvalueCount = function->upvalueCount;
  return closure;
}

ObjFunction *newFunction() {
  ObjFunction *function  = ALLOCATE_OBJ(ObjFunction, OBJ_FUNCTION);
  function->arity        = 0;
  function->upvalueCount = 0;
  function->name         = NULL;
  initChunk(&function->chunk);
  return function;
}

ObjNative *newNative(NativeFn function) {
  ObjNative *native = ALLOCATE_OBJ(ObjNative, OBJ_NATIVE);
  native->function  = function;
  return native;
}

static ObjString *allocateString(char *chars, int length, uint32_t hash) {
  ObjString *string = ALLOCATE_OBJ(ObjString, OBJ_STRING);
  string->length    = length;
  string->chars     = chars;
  string->hash      = hash;

  push(OBJ_VAL(string));
  tableSet(&vm.strings, string, NIL_VAL);
  pop();
  return string;
}

static uint32_t hashString(const char *key, int length) {
  uint32_t hash = 2166136261u;
  for (int i = 0; i < length; i++) {
    hash ^= (uint8_t)key[i];
    hash *= 16777619;
  }
  return hash;
}

ObjString *takeString(char *chars, int length) {
  uint32_t hash       = hashString(chars, length);
  ObjString *interned = tableFindString(&vm.strings, chars, length, hash);

  if (interned != NULL) {
    FREE_ARRAY(char, chars, length + 1);
    return interned;
  }

  return allocateString(chars, length, hash);
}

ObjString *copyString(const char *chars, int length) {
  uint32_t hash       = hashString(chars, length);
  ObjString *interned = tableFindString(&vm.strings, chars, length, hash);

  if (interned != NULL)
    return interned;

  char *heapChars = ALLOCATE(char, length + 1);
  memcpy(heapChars, chars, length);
  heapChars[length] = '\0';
  return allocateString(heapChars, length, hash);
}

ObjUpvalue *newUpvalue(Value *slot) {
  ObjUpvalue *upvalue = ALLOCATE_OBJ(ObjUpvalue, OBJ_UPVALUE);
  upvalue->closed     = NIL_VAL;
  upvalue->location   = slot;
  upvalue->next       = NULL;
  return upvalue;
}

static void printFunction(ObjFunction *function) {
  if (function->name == NULL) {
    printf("<script>");
    return;
  }
  printf("<fn %s>", function->name->chars);
}

void printObject(Value value) {
  switch (OBJ_TYPE(value)) {
    case OBJ_CLOSURE : printFunction(AS_CLOSURE(value)->function); break;
    case OBJ_FUNCTION: printFunction(AS_FUNCTION(value)); break;
    case OBJ_NATIVE  : printf("<native fn>"); break;
    case OBJ_STRING  : printf("%s", AS_CSTRING(value)); break;
    case OBJ_UPVALUE : printf("Upvalue"); break;
  }
}
