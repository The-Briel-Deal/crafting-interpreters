#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "memory.h"
#include "table.h"
#include "value.h"

#define TABLE_MAX_LOAD 0.75

void initTable(Table *table) {
  table->count    = 0;
  table->capacity = 0;
  table->entries  = NULL;
}
void freeTable(Table *table) {
  FREE_ARRAY(Entry, table->entries, table->capacity);
  initTable(table);
}

static Entry *findEntry(Entry *entries, int capacity, Value key) {
  switch (key.type) {
    case VAL_OBJ: {
      ObjString *keyStr    = AS_STRING(key);
      uint32_t   index     = keyStr->hash % capacity;
      Entry     *tombstone = NULL;
      for (;;) {
        Entry *entry = &entries[index];
        if (IS_NIL(entry->key)) {
          if (IS_NIL(entry->value)) {
            return tombstone != NULL ? tombstone : entry;
          } else {
            if (tombstone == NULL)
              tombstone = entry;
          }
        } else if (AS_STRING(entry->key) == keyStr) {
          return entry;
        }

        index = (index + 1) % capacity;
      }
    }
    case VAL_NUMBER: {
      int    keyNum    = key.as.number;
      int    index     = keyNum % capacity;
      Entry *tombstone = NULL;
      for (;;) {
        Entry *entry = &entries[index];
        if (IS_NIL(entry->key)) {
          if (IS_NIL(entry->value)) {
            return tombstone != NULL ? tombstone : entry;
          } else {
            if (tombstone == NULL)
              tombstone = entry;
          }
        } else if (IS_NUMBER(entry->key) && AS_NUMBER(entry->key) == keyNum) {
          return entry;
        }

        index = (index + 1) % capacity;
      }
    }
  }
}

static void adjustCapacity(Table *table, int capacity) {
  Entry *entries = ALLOCATE(Entry, capacity);
  for (int i = 0; i < capacity; i++) {
    entries[i].key   = NIL_VAL;
    entries[i].value = NIL_VAL;
  }

  table->count = 0;

  for (int i = 0; i < table->capacity; i++) {
    Entry *entry = &table->entries[i];
    if (IS_NIL(entry->key))
      continue;
    Entry *dest = findEntry(entries, capacity, entry->key);
    dest->key   = entry->key;
    dest->value = entry->value;
    table->count++;
  }

  FREE_ARRAY(Entry, table->entries, table->capacity);
  table->entries  = entries;
  table->capacity = capacity;
}

bool tableSet(Table *table, Value key, Value value) {
  if (table->count + 1 > table->capacity * TABLE_MAX_LOAD) {
    int capacity = GROW_CAPACITY(table->capacity);
    adjustCapacity(table, capacity);
  }
  Entry *entry    = findEntry(table->entries, table->capacity, key);
  bool   isNewKey = IS_NIL(entry->key);
  if (isNewKey)
    table->count++;

  entry->key   = key;
  entry->value = value;
  return isNewKey;
}

bool tableDelete(Table *table, Value key) {
  if (table->count == 0)
    return false;

  Entry *entry = findEntry(table->entries, table->capacity, key);
  if (IS_NIL(entry->key))
    return false;

  entry->key   = NIL_VAL;
  entry->value = BOOL_VAL(true);
  return true;
}

bool tableGet(Table *table, Value key, Value *value) {
  if (table->count == 0)
    return false;
  Entry *entry = findEntry(table->entries, table->capacity, key);

  if (IS_NIL(entry->key))
    return false;
  *value = entry->value;
  return true;
}

void tableAddAll(Table *from, Table *to) {
  for (int i = 0; i < from->capacity; i++) {
    Entry *entry = &from->entries[i];
    if (!IS_NIL(entry->key)) {
      tableSet(to, entry->key, entry->value);
    }
  }
}

ObjString *tableFindString(Table *table, const char *chars, int length,
                           uint32_t hash) {
  if (table->count == 0)
    return NULL;

  uint32_t index = hash % table->capacity;

  for (;;) {
    Entry *entry = &table->entries[index];
    if (IS_NIL(entry->key)) {
      if (IS_NIL(entry->value))
        return NULL;
    } else if (AS_STRING(entry->key)->length == length &&
               AS_STRING(entry->key)->hash == hash &&
               memcmp(AS_STRING(entry->key)->chars, chars, length) == 0) {
      return AS_STRING(entry->key);
    }

    index = (index + 1) % table->capacity;
  }
}
