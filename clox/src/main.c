#include "vm.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

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
#define CURSOR_FORWARD(n) printf("\x1b[%iC", n)
#define CURSOR_BACK(n) printf("\x1b[%iD", n)
  char line[1024];
  uint16_t line_index = 0;
  printf("> ");
  enableRawMode();
  for (;;) {
    char c;
    while (read(STDIN_FILENO, &c, 1)) {
      if (iscntrl(c)) {
        // Control Characters
        switch (c) {
          case '\n':
          case '\r': {
            line[line_index] = '\0';
            line_index       = 0;
            printf("\r\n> ");
            break;
          }
          case CTRL('c'): exit(EXIT_FAILURE); break;
          default       : printf("%i", c); break;
        }
      } else {
				if (c == 'h') {
					CURSOR_BACK(1);
					continue;
				}
				if (c == 'l') {
					CURSOR_FORWARD(1);
					continue;
				}
        // Normal Chars
        line[line_index++] = c;
        printf("%c", c);
      }
    }

    interpret(line);
  }
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
