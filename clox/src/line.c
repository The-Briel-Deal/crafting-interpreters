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
  for (int i = line->length - 1; i >= line->pos; i--) {
    line->start[i + 1] = line->start[i];
  }
  line->length++;
  line->start[line->pos] = c;
  line->pos++;
}

void setPosLine(struct Line *line, int index) {
  assert(index >= 0);
  assert(index < line->length);
  line->pos = index;
}

void freeLine(struct Line *line) {
  if (line->start) {
    free(line->start);
  }
}
