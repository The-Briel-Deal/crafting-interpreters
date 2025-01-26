#include <stdio.h>

#include "chunk.h"
#include "scanner.h"

void compile(const char *source, Chunk *chunk) {
  initScanner(source);
  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression.");
}
