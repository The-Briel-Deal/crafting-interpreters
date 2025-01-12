#include <stdio.h>
#include <unistd.h>

static int testWriteChunk();
int main(int argc, char *argv[]) {
  printf("Starting Tests (:\n");
  testWriteChunk();
  printf("Tests Succeeded!\n");
}

static int testWriteChunk() {
  FILE *prev_stdout = stdout;
  char buf[1000];
  FILE *membufstdout = fmemopen(buf, sizeof(buf), "w");
  printf("Start position of mem buffer: %ld\n", ftell(membufstdout));
  stdout = membufstdout;
  printf("Cute Beans");
  long end_pos = ftell(membufstdout);
  fclose(stdout);
  stdout = prev_stdout;
  printf("End position of mem buffer: %ld\n", end_pos);
  return 0;
}
