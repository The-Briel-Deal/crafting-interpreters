#include <assert.h>

#include "compiler.h"
#include "helper_test.h"
#include "vm.h"

const char TEST_ARITHMETIC_EXPECT[] = "-0.821429\n";
void       testArithmetic() {
	initVM();
  char source[] = "-((1.2 + 3.4) / 5.6)";
  interpret(source);
}

const TestCase VM_TESTS[] = {
    {"testArithmetic", testArithmetic, TEST_ARITHMETIC_EXPECT},
};

const int VM_TESTS_COUNT = sizeof(VM_TESTS) / sizeof(TestCase);
