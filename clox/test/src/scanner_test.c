#include <stdio.h>

#include "scanner.h"
#include "vm.h"

static void scanAndPrintTokens(char *source);

const char TEST_SCANNER_EXPECT[] = "   1 31 'print'\n"
                                   "   | 21 '1'\n"
                                   "   |  7 '+'\n"
                                   "   | 21 '2'\n"
                                   "   |  8 ';'\n"
                                   "   2 39 ''\n";
void testScanner() {
  initVM();
  char source[] = "print 1 + 2;\n";
  scanAndPrintTokens(source);
}

// Helper
static void scanAndPrintTokens(char *source) {
  initScanner(source);
  int line = -1;
  for (;;) {
    Token token = scanToken();
    if (token.line != line) {
      printf("%4d ", token.line);
      line = token.line;
    } else {
      printf("   | ");
    }
    printf("%2d '%.*s'\n", token.type, token.length, token.start);

    if (token.type == TOKEN_EOF)
      break;
  }
}
