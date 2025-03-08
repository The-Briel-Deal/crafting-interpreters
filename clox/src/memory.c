#include <stdlib.h>

#include "chunk.h"
#include "compiler.h"
#include "memory.h"
#include "object.h"
#include "value.h"
#include "vm.h"

#ifdef DEBUG_LOG_GC
#include "debug.h"
#include <stdio.h>
#endif

#define GC_HEAP_GROW_FACTOR 2

void *reallocate(void *pointer, size_t oldSize, size_t newSize) {
  //! TODO: Re-enable GC once mark compact impl is done.
  //  vm.bytesAllocated += newSize - oldSize;
  //  if (newSize > oldSize) {
  // #ifdef DEBUG_STRESS_GC
  //    collectGarbage();
  // #endif
  //    if (vm.bytesAllocated > vm.nextGC) {
  //      collectGarbage();
  //    }
  //  }

  if (newSize == 0) {
    free(pointer);
    return NULL;
  }

  void *result = realloc(pointer, newSize);
  if (result == NULL)
    exit(1);
  return result;
}

void markObject(Obj *object) {
  if (object == NULL)
    return;
  if (object->isMarked)
    return;
#ifdef DEBUG_LOG_GC
  printf("%p mark ", (void *)object);
  printValue(OBJ_VAL(object));
  printf("\n");
#endif

  object->isMarked = true;

  if (vm.grayCapacity < vm.grayCount + 1) {
    vm.grayCapacity = GROW_CAPACITY(vm.grayCapacity);
    vm.grayStack =
        (Obj **)realloc(vm.grayStack, sizeof(Obj *) * vm.grayCapacity);

    if (vm.grayStack == NULL)
      exit(1);
  }

  vm.grayStack[vm.grayCount++] = object;
}

void markValue(Value value) {
  if (IS_OBJ(value))
    markObject(AS_OBJ(value));
}

static void markArray(ValueArray *array) {
  for (int i = 0; i < array->count; i++) {
    markValue(array->values[i]);
  }
}

static void blackenObject(Obj *object) {
#ifdef DEBUG_LOG_GC
  printf("%p blacken ", (void *)object);
  printValue(OBJ_VAL(object));
  printf("\n");
#endif
  switch (object->type) {
    case OBJ_CLOSURE: {
      ObjClosure *closure = (ObjClosure *)object;
      markObject((Obj *)closure->function);
      for (int i = 0; i < closure->upvalueCount; i++) {
        markObject((Obj *)closure->upvalues[i]);
      }
      break;
    }
    case OBJ_FUNCTION: {
      ObjFunction *function = (ObjFunction *)object;
      markObject((Obj *)function->name);
      markArray(&function->chunk.constants);
      break;
    }
    case OBJ_UPVALUE: markValue(((ObjUpvalue *)object)->closed); break;
    case OBJ_NATIVE :
    case OBJ_STRING : break;
  }
}

static void markRoots() {
  for (Value *slot = vm.stack; slot < vm.stackTop; slot++) {
    markValue(*slot);
  }

  for (int i = 0; i < vm.frameCount; i++) {
    markObject((Obj *)vm.frames[i].closure);
  }

  for (ObjUpvalue *upvalue = vm.openUpvalues; upvalue != NULL;
       upvalue             = upvalue->next) {
    markObject((Obj *)upvalue);
  }

  markTable(&vm.globals);
  markCompilerRoots();
}

static void traceReferences() {
  while (vm.grayCount > 0) {
    Obj *object = vm.grayStack[--vm.grayCount];
    blackenObject(object);
  }
}

static bool gcDisabled = false;

void disableGC() {
  gcDisabled = true;
}

void enableGC() {
  gcDisabled = false;
}

void collectGarbage() {
  if (gcDisabled)
    return;
#ifdef DEBUG_LOG_GC
  printf("-- gc begin\n");
  size_t before = vm.bytesAllocated;
#endif

  // To move to mark compact we need to replace the call to sweep with a
  // function that calculates a new position for all unmarked objects. Then we
  // need a function that updates references to these objects with their new
  // positions.
  // * In `stack` we need to walk the stack and update the references to All
  //   VAL_OBJ objects.
  // * In `frames` we need to walk through every call frame and update refs to
  //   `ObjClosure`.
  // * In `globals` and `strings` we need to walk through all living entries and
  //   updating their ObjString keys, then all of their values of type VAL_OBJ.
  // * Finally we walk all objects and create a function similar to markObj
  //   where we update it's addr.
  //
  // Then after all refs are updated, we move all objects to their calculated
  // position.

  // Mark
  markRoots();
  traceReferences();
  //! Old Sweep
  // tableRemoveWhite(&vm.strings);
  // sweep();
	//
  //! Compact
  // calculateNewPos();
  // updateObjRefs();
  // compact();

  // vm.nextGC = vm.bytesAllocated * GC_HEAP_GROW_FACTOR;

#ifdef DEBUG_LOG_GC
  printf("-- gc end\n");
  printf("   collected %zu bytes (from %zu to %zu) next at %zu\n",
         before - vm.bytesAllocated, before, vm.bytesAllocated, vm.nextGC);
#endif
}
