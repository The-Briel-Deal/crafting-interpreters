#include <assert.h>

#include "helper_test.h"
#include "vm.h"

const char TEST_ARITHMETIC_1_EXPECT[] = "-0.821429\n";
void       testArithmetic1() {
  initVM();
  char source[] = "print(-((1.2 + 3.4) / 5.6));";
  interpret(source);
}

const char TEST_ARITHMETIC_2_EXPECT[] = "9\n";
void       testArithmetic2() {
  initVM();
  char source[] = "print(3 + 3 * 2);";
  interpret(source);
}

const char TEST_ARITHMETIC_3_EXPECT[] = "1363.79\n";
void       testArithmetic3() {
  initVM();
  char source[] = "print(10000 / 1.9 * 6 / 24 + 48);";
  interpret(source);
}

const char TEST_BOOLEAN_LOGIC_1_EXPECT[] = "false\n";
void       testBooleanLogic1() {
  initVM();
  char source[] = "print(!!!!!true);";
  interpret(source);
}

const char TEST_BOOLEAN_LOGIC_2_EXPECT[] = "true\n";
void       testBooleanLogic2() {
  initVM();
  char source[] = "print(!(5 - 4 > 3 * 2 == !nil));";
  interpret(source);
}

const char TEST_STRING_CONCAT_1_EXPECT[] = "Boogie Woogie Woohoo\n";
void       testStringConcat1() {
  initVM();
  char source[] = "print(\"Boogie \" + \"Woogie \" + \"Woohoo\");";
  interpret(source);
}
const TestCase VM_TESTS[] = {
    {"testArithmetic1",   testArithmetic1,   TEST_ARITHMETIC_1_EXPECT   },
    {"testArithmetic2",   testArithmetic2,   TEST_ARITHMETIC_2_EXPECT   },
    {"testArithmetic3",   testArithmetic3,   TEST_ARITHMETIC_3_EXPECT   },
    {"testBooleanLogic1", testBooleanLogic1, TEST_BOOLEAN_LOGIC_1_EXPECT},
    {"testBooleanLogic2", testBooleanLogic2, TEST_BOOLEAN_LOGIC_2_EXPECT},
    {"testStringConcat1", testStringConcat1, TEST_STRING_CONCAT_1_EXPECT},
};

const int VM_TESTS_COUNT = sizeof(VM_TESTS) / sizeof(TestCase);
