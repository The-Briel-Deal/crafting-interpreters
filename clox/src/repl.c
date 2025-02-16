#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "line.h"

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
         (int)(line->pos + sizeof(PROMPT_PREFIX) - 1));
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

char *Repl_readLine() {
  struct Line line;
  initLine(&line);
  redrawLine(&line);

  bool not_ready = true;
  while (not_ready) {
    char c = getchar();
    switch (c) {
      case '\n':
      case '\r'     : not_ready = false; continue;
      case CTRL('c'): exit(1);
      case CTRL('h'):
        setPosLine(&line, line.pos - 1);
        redrawLine(&line);
        continue;
      case CTRL('l'):
        setPosLine(&line, line.pos + 1);
        redrawLine(&line);
        continue;
      case ANSII_DEL:
        removeLine(&line);
        redrawLine(&line);
        continue;
      case ANSII_ESC:
        c = getchar();
        // If next character isn't OPEN_BRAC then this isn't an escape code and
        // ESC doesn't do anything.
        if (c != OPEN_BRAC) {
          ungetc(c, stdin);
          continue;
        }

        c = getchar();

        int digitInt = 1;
        if (isdigit(c)) {
          ungetc(c, stdin);
          digitInt = getNumStdin();
        }
        if (c == 'D') {
          setPosLine(&line, line.pos - digitInt);
          redrawLine(&line);
        }
        if (c == 'C') {
          setPosLine(&line, line.pos + digitInt);
          redrawLine(&line);
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
  line.start[line.length] = '\0';
  return line.start;
}
