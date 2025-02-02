#ifndef clox_scanner_test_h
#define clox_scanner_test_h

const char TEST_SCANNER_EXPECT[] = "   1 31 'print'\n"
                                   "   | 21 '1'\n"
                                   "   |  7 '+'\n"
                                   "   | 21 '2'\n"
                                   "   |  8 ';'\n"
                                   "   2 39 ''\n";
void testScanner();

#endif
