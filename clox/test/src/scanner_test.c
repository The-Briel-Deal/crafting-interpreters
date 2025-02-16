#include <stdio.h>

#include "helper_test.h"
#include "scanner.h"
#include "vm.h"

static void scanAndPrintTokens(char *source);

const char TEST_SCANNER_IPRINT_EXPECT[] =
    "   1 32 'print'\n"
    "   | 22 '1'\n"
    "   |  7 '+'\n"
    "   | 22 '2'\n"
    "   |  9 ';'\n"
    "   2 42 ''\n";
void testScannerIPrint() {
  initVM();
  char source[] = "print 1 + 2;\n";
  scanAndPrintTokens(source);
}

const char TEST_SCANNER_SWITCH_EXPECT[] =
    "   1 31 'print'\n"
    "   | 21 '1'\n"
    "   |  7 '+'\n"
    "   | 21 '2'\n"
    "   |  8 ';'\n"
    "   2 39 ''\n";
void testScannerSwitch() {
  initVM();
  char source[] =
      "var i = 2;                   \n"
      "switch (i) {                 \n"
      "  case 1:                    \n"
      "    print(\"is one\");       \n"
      "  case 2:                    \n"
      "  	 print(\"is two\");       \n"
      "  case 3:                    \n"
      "  	 print(\"is three\");     \n"
      "  default:                   \n"
      "  	 print(\"default case\"); \n"
      "}                            \n";
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
    {"testScannerIPrint", testScannerIPrint, TEST_SCANNER_IPRINT_EXPECT},
    {"testScannerSwitch", testScannerSwitch, TEST_SCANNER_SWITCH_EXPECT},
};

const int SCANNER_TESTS_COUNT = sizeof(SCANNER_TESTS) / sizeof(TestCase);
