#include "vm.h"
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define ANSII_DEL 127
#define ANSII_ESC 27
#define OPEN_BRAC 91

#define CLEAR_LINE()                                                           \
  do {                                                                         \
    putchar(ANSII_ESC);                                                        \
    putchar(OPEN_BRAC);                                                        \
    printf("2K");                                                              \
  } while (false)

static void setCursorPos(int line, int col) {
  putchar(ANSII_ESC);
  putchar(OPEN_BRAC);
  printf("%i;%iH", line, col);
}

static void redrawLine(char *line) {
  CLEAR_LINE();
  putchar('\r');
  printf("> %s", line);
}

struct termios orig_termios;

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
  setvbuf(stdout, NULL, _IONBF, 0);
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);

  struct termios raw = orig_termios;

  raw.c_iflag &= ~(ICRNL | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int deleteChar(int index, char line[1024]) {
  if (index <= 0)
    return index;
  for (int i = index - 1; line[i] != '\0'; i++) {
    line[i] = line[i + 1];
  }
  index--;
  redrawLine(line);
  return index;
}

static void repl() {
#define CURSOR_BACK()                                                          \
  do {                                                                         \
    putchar(ANSII_ESC);                                                        \
    putchar(OPEN_BRAC);                                                        \
    printf("1D");                                                              \
    index--;                                                                   \
  } while (false)

#define CURSOR_FORWARD()                                                       \
  do {                                                                         \
    putchar(ANSII_ESC);                                                        \
    putchar(OPEN_BRAC);                                                        \
    printf("1C");                                                              \
    index++;                                                                   \
  } while (false)

  enableRawMode();
  char line[1024];
  int index = 0;
  for (;;) {
    bool not_ready = true;
    while (not_ready) {
      char c = getchar();
      switch (c) {
        case '\n':
        case '\r':
          line[index] = '\0';
          not_ready   = false;
          continue;
        case CTRL('c'): exit(1);
        case CTRL('h'): CURSOR_BACK(); continue;
        case CTRL('l'): CURSOR_FORWARD(); continue;
        case ANSII_DEL: index = deleteChar(index, line); continue;
        case ANSII_ESC:
          c = getchar();
          assert(c == OPEN_BRAC);

          uint8_t digitIndex = 0;
          char digitStr[8];
          while (c = getchar(), isdigit(c)) {
            digitStr[digitIndex++] = c;
          }
          digitStr[digitIndex] = '\0';

          // If no count, default to 1.
          int digitInt = 1;
          if (digitIndex != 0) {
            digitInt = atoi(digitStr);
          }
          if (c == 'D') {
            for (int _i = 0; _i < digitInt; _i++) {
              CURSOR_BACK();
              index--;
            }
          }
          if (c == 'C') {
            for (int _i = 0; _i < digitInt; _i++) {
              CURSOR_FORWARD();
              index++;
            }
          }
          continue;
        default:
          if (isprint(c)) {
            line[index++] = c;
            line[index]   = '\0';
            redrawLine(line);
            continue;
          }
      }
    }
    printf("\r\n");
    interpret(line);
    index       = 0;
    line[index] = '\0';
    printf("\r\n");
    redrawLine(line);
  }
#undef CURSOR_BACK
#undef CURSOR_FORWARD
}

static char *readFile(const char *path) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file \"%s\".\n", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t fileSize = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(fileSize + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
    exit(74);
  }

  size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
  if (bytesRead < fileSize) {
    fprintf(stderr, "Could not read file \"%s\".", path);
    exit(74);
  }

  buffer[bytesRead] = '\0';

  fclose(file);
  return buffer;
}

static void runFile(const char *path) {
  char *source           = readFile(path);
  InterpretResult result = interpret(source);
  free(source);

  if (result == INTERPRET_COMPILE_ERROR)
    exit(65);
  if (result == INTERPRET_RUNTIME_ERROR)
    exit(70);
}

int main(int argc, char *argv[]) {
  initVM();

  if (argc == 1) {
    repl();
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    fprintf(stderr, "Usage: clox [path]");
    exit(64);
  }

  freeVM();

  return 0;
}
