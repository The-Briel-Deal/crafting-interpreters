#include <stdio.h>

#include "chunk.h"
#include "scanner.h"

typedef struct {
  Token current;
  Token previous;
} Parser;

Parser parser;

static void advance() {
  parser.previous = parser.current;
  for (;;) {
    parser.current = scanToken();
    if (parser.current.type != TOKEN_ERROR)
      break;

    errorAtCurrent(parser.current.start);
  }
}

void compile(const char *source, Chunk *chunk) {
  initScanner(source);
  advance();
  expression();
  consume(TOKEN_EOF, "Expect end of expression.");
}
