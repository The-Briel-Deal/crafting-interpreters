#include <stdio.h>

#include "helper_test.h"
#include "scanner.h"
#include "vm.h"

static void scanAndPrintTokens(char *source);

const char TEST_SCANNER_EXPECT[] =
    "   1 32 'print'\n"
    "   | 21 '1'\n"
    "   |  7 '+'\n"
    "   | 21 '2'\n"
    "   |  8 ';'\n"
    "   2 40 ''\n";

void testScanner() {
  initVM();
  char source[] = "print 1 + 2;\n";
  scanAndPrintTokens(source);
}

const char TEST_SCANNER_CONTINUE_EXPECT[] =
    "   1 27 'for'\n"
    "   |  0 '('\n"
    "   | 37 'var'\n"
    "   | 19 'i'\n"
    "   | 13 '='\n"
    "   | 21 '0'\n"
    "   |  8 ';'\n"
    "   | 19 'i'\n"
    "   | 17 '<'\n"
    "   | 21 '10'\n"
    "   |  8 ';'\n"
    "   | 19 'i'\n"
    "   | 13 '='\n"
    "   | 19 'i'\n"
    "   |  7 '+'\n"
    "   | 21 '1'\n"
    "   |  1 ')'\n"
    "   |  2 '{'\n"
    "   2 24 'continue'\n"
    "   |  8 ';'\n"
    "   3  3 '}'\n"
    "   4 40 ''\n";

void testScannerContinue() {
  initVM();
  char source[] =
      "for (var i = 0; i < 10; i = i + 1) {\n"
      "  continue;\n"
      "}\n";
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

const TestCase SCANNER_TESTS[] = {
    {"testScanner",         testScanner,         TEST_SCANNER_EXPECT         },
    {"testScannerContinue", testScannerContinue, TEST_SCANNER_CONTINUE_EXPECT},
};

const int SCANNER_TESTS_COUNT = sizeof(SCANNER_TESTS) / sizeof(TestCase);
