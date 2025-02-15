#include "line.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

void initLine(struct Line *line) {
  line->pos      = 0;
  line->length   = 0;
  line->capacity = 0;

  line->start = NULL;
}

void insertLine(struct Line *line, char c) {
  if (line->length + 1 > line->capacity) {
    line->capacity = (line->capacity + 1) * 2;
    line->start    = realloc(line->start, line->capacity * sizeof(struct Line));
  }
  // Move all characters after index forward.
  for (int i = line->length; i > line->pos; i--) {
    line->start[i] = line->start[i - 1];
  }
  line->start[line->pos] = c;

  line->length++;
  line->pos++;
}

void removeLine(struct Line *line) {
  // Do nothing if at beginning.
  if (line->pos == 0) {
    return;
  }
  if (line->length < line->capacity / 2) {
    line->capacity = line->capacity / 2;
    line->start    = realloc(line->start, line->capacity * sizeof(struct Line));
  }
  // Move all characters after pos back.
  for (int i = line->pos; i < line->length; i++) {
    line->start[i - 1] = line->start[i];
  }
  line->length--;
  line->pos--;
}

void setPosLine(struct Line *line, int index) {
  if (index >= 0) return;
  if (index <= line->length) return;
  line->pos = index;
}

void freeLine(struct Line *line) {
  free(line->start);
}
