#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "object.h"
#include "value.h"
#include "vm.h"

#define BENCH_RUN_COUNT         2000
#define BENCH_TABLE_ENTRY_COUNT 2000

long benchStrConcat();
long benchTable();
long benchVars();
void benchFibonacci();
static void runBench(long (*benchFn)(), FILE *stdoutRedirect);

int main(int argc, char *argv[]) {
  FILE *stdoutRedirect = fopen("./benchmark_out.txt", "w");
  printf("benchStrConcat:\n");
  runBench(benchStrConcat, stdoutRedirect);
  printf("benchTable:\n");
  runBench(benchTable, stdoutRedirect);
  printf("benchVars:\n");
  runBench(benchVars, stdoutRedirect);
  fclose(stdoutRedirect);

  benchFibonacci();
}

static void runBench(long (*benchFn)(), FILE *stdoutRedirect) {
  FILE *stdoutPrev             = stdout;
  stdout                       = stdoutRedirect;
  int results[BENCH_RUN_COUNT] = {0};
  for (int i = 0; i < BENCH_RUN_COUNT; i++) {
    results[i] = benchFn();
  }
  int max   = INT_MIN;
  int min   = INT_MAX;
  float avg = 0;
  for (int i = 0; i < BENCH_RUN_COUNT; i++) {
    avg += results[i] / (float)BENCH_RUN_COUNT;
    if (results[i] < min) {
      min = results[i];
    }
    if (results[i] > max) {
      max = results[i];
    }
  }
  stdout = stdoutPrev;

  printf("Min: %i\nMax: %i\nAvg: %f\n\n", min, max, avg);
}

const char VERY_LONG_STR_CONCAT_SCRIPT[];
long benchStrConcat() {
  initVM();
  long startTime = clock();
  interpret(VERY_LONG_STR_CONCAT_SCRIPT);
  long endTime = clock();
  printf("\nStart Time: %li\nEnd Time: %li\nElapsed Time: %li\n", startTime,
         endTime, endTime - startTime);
  return endTime - startTime;
}

long benchVars() {
  char source[] =
      "var a = \"foo\";\n"
      "{\n"
      "  var b = \"bar\";\n"
      "	 a = \"boo\";\n"
      "	 {\n"
      "	   var c = \"chong\";\n"
      "	   b = \"baz\";\n"
      "	 }\n"
      "	 var d = \"woahg\";\n"
      "	 print(a + b);\n"
      "}";
  initVM();
  long startTime = clock();
  interpret(source);
  long endTime = clock();
  printf("\nStart Time: %li\nEnd Time: %li\nElapsed Time: %li\n", startTime,
         endTime, endTime - startTime);
  return endTime - startTime;
}

long benchTable() {
  long startTime = clock();
  Table table;
  initTable(&table);
  for (int i = 0; i < BENCH_TABLE_ENTRY_COUNT; i++) {
    char key[32]      = {0};
    int keyLen        = sprintf(key, "testKey%i", i);
    ObjString *keyObj = copyString(key, keyLen);
    char val[32]      = {0};
    int valLen        = sprintf(val, "testVal%i", i);
    ObjString *valObj = copyString(val, valLen);

    bool isNewKey = tableSet(&table, keyObj, OBJ_VAL(valObj));
    assert(isNewKey);
  }

  for (int i = 0; i < BENCH_TABLE_ENTRY_COUNT; i++) {
    char key[32]      = {0};
    int keyLen        = sprintf(key, "testKey%i", i);
    ObjString *keyObj = copyString(key, keyLen);
    Value resultVal;
    bool foundKey = tableGet(&table, keyObj, &resultVal);
    assert(foundKey);

    ObjString *resultObjStr = AS_STRING(resultVal);
    char expectVal[32]      = {0};
    int expectValLen        = sprintf(expectVal, "testVal%i", i);

    assert(resultObjStr->length == expectValLen);
    assert(memcmp(resultObjStr->chars, expectVal, expectValLen) == 0);
    printf("%s -> %s", key, resultObjStr->chars);
  }
  long endTime = clock();
  return endTime - startTime;
}

void benchFibonacci() {
  initVM();
  char source[] =
      "fun fib(n) {\n"
      "  if (n < 2) return n;\n"
      "  return fib(n - 2) + fib(n - 1);\n"
      "}\n"
      "\n"
      "var start = clock();\n"
      "print fib(35);\n"
      "print clock() - start;\n";

  interpret(source);
  freeVM();
}

const char VERY_LONG_STR_CONCAT_SCRIPT[] =
    "print(\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \");";
