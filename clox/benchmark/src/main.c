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

long        benchStrConcat();
long        benchTable();
static void runBench(long (*benchFn)(), FILE *stdoutRedirect);

int main(int argc, char *argv[]) {
  FILE *stdoutRedirect = fopen("./benchmark_out.txt", "w");
  runBench(benchStrConcat, stdoutRedirect);
  runBench(benchTable, stdoutRedirect);
  fclose(stdoutRedirect);
}

static void runBench(long (*benchFn)(), FILE *stdoutRedirect) {
  FILE *stdoutPrev             = stdout;
  stdout                       = stdoutRedirect;
  int results[BENCH_RUN_COUNT] = {0};
  for (int i = 0; i < BENCH_RUN_COUNT; i++) {
    results[i] = benchFn();
  }
  int   max = INT_MIN;
  int   min = INT_MAX;
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

  printf("benchStrConcat:\nMin: %i\nMax: %i\nAvg: %f\n", min, max, avg);
}

const char VERY_LONG_STR_CONCAT_SCRIPT[];
long       benchStrConcat() {
  initVM();
  long startTime = clock();
  interpret(VERY_LONG_STR_CONCAT_SCRIPT);
  long endTime = clock();
  printf("\nStart Time: %li\nEnd Time: %li\nElapsed Time: %li\n", startTime,
               endTime, endTime - startTime);
  return endTime - startTime;
}

long benchTable() {
  long  startTime = clock();
  Table table;
  initTable(&table);
  for (int i = 0; i < BENCH_TABLE_ENTRY_COUNT; i++) {
    char       key[32] = {0};
    int        keyLen  = sprintf(key, "testKey%i", i);
    ObjString *keyObj  = copyString(key, keyLen);
    char       val[32] = {0};
    int        valLen  = sprintf(val, "testVal%i", i);
    ObjString *valObj  = copyString(val, valLen);

    bool isNewKey = tableSet(&table, keyObj, OBJ_VAL(valObj));
  }

  for (int i = 0; i < BENCH_TABLE_ENTRY_COUNT; i++) {
    char       key[32] = {0};
    int        keyLen  = sprintf(key, "testKey%i", i);
    ObjString *keyObj  = copyString(key, keyLen);
    Value      resultVal;
    bool       foundKey = tableGet(&table, keyObj, &resultVal);

    ObjString *resultObjStr  = AS_STRING(resultVal);
    char       expectVal[32] = {0};
    int        expectValLen  = sprintf(expectVal, "testVal%i", i);

    assert(resultObjStr->length == expectValLen);
    assert(memcmp(resultObjStr->chars, expectVal, expectValLen) == 0);
    printf("%s -> %s", key, resultObjStr->chars);
  }
  long endTime = clock();
  return endTime - startTime;
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
