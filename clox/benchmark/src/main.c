#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "vm.h"

#define BENCH_RUN_COUNT 2000

long benchStrConcat();

int main(int argc, char *argv[]) {
  int results[BENCH_RUN_COUNT] = {0};
  for (int i = 0; i < BENCH_RUN_COUNT; i++) {
    results[i] = benchStrConcat();
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

  printf("benchStrConcat:\n\nMin: %i\nMax: %i\nAvg: %f\n", min, max, avg);
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
