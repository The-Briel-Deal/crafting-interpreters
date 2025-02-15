#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "line.h"
#include "vm.h"

#define PROMPT_PREFIX "> "

#define ANSII_DEL     127
#define ANSII_ESC     27
#define OPEN_BRAC     91

#define CLEAR_LINE()                                                           \
  do {                                                                         \
    putchar(ANSII_ESC);                                                        \
    putchar(OPEN_BRAC);                                                        \
    printf("2K");                                                              \
  } while (false)

static int getNumStdin() {
  uint8_t digitIndex = 0;
  char digitStr[10];
  char c;
  while (c = getchar(), isdigit(c)) {
    digitStr[digitIndex++] = c;
  }
  digitStr[digitIndex] = '\0';
  ungetc(c, stdin);

  // If no count, default to 1.
  int digitInt = 1;
  if (digitIndex != 0) {
    digitInt = atoi(digitStr);
  }
  return digitInt;
}

static void redrawLine(struct Line *line) {
  CLEAR_LINE();
  putchar('\r');
  printf("%s%.*s", PROMPT_PREFIX, line->length, line->start);
  putchar('\r');
  printf("%c%c%iC", ANSII_ESC, OPEN_BRAC,
         (int)(line->pos + sizeof(PROMPT_PREFIX)));
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
  struct Line line;
  initLine(&line);
  for (;;) {
    bool not_ready = true;
    while (not_ready) {
      char c = getchar();
      switch (c) {
        case '\n':
        case '\r'     : not_ready = false; continue;
        case CTRL('c'): exit(1);
        case CTRL('h'): setPosLine(&line, line.pos - 1); continue;
        case CTRL('l'): setPosLine(&line, line.pos + 1); continue;
        case ANSII_DEL: removeLine(&line); continue;
        case ANSII_ESC:
          c = getchar();
          assert(c == OPEN_BRAC);

          int digitInt = getNumStdin();
          if (c == 'D') {
            for (int _i = 0; _i < digitInt; _i++) {
              setPosLine(&line, line.pos - 1);
            }
          }
          if (c == 'C') {
            for (int _i = 0; _i < digitInt; _i++) {
              setPosLine(&line, line.pos + 1);
            }
          }
          continue;
        default:
          if (isprint(c)) {
            insertLine(&line, c);
            redrawLine(&line);
            continue;
          }
      }
    }
    printf("\r\n");
    // TODO: Make terminate line helper func.
    line.start[line.length] = '\0';
    interpret(line.start);
    initLine(&line);
    printf("\r\n");
    redrawLine(&line);
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
