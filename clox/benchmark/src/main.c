#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "vm.h"

#define BENCH_RUN_COUNT 2000

long        benchStrConcat();
static void runBench(long (*benchFn)(), FILE *stdoutRedirect);

int main(int argc, char *argv[]) {
  FILE *stdoutRedirect = fopen("./benchmark_out.txt", "w");
  runBench(benchStrConcat, stdoutRedirect);
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

const char VERY_LONG_STR_CONCAT_SCRIPT[] =
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
    "\"Boogie \" + \"Woogie \" + \"Woohoo \" +"
    "\"Boogie \" + \"Woogie \" + \"Woohoo \"";
