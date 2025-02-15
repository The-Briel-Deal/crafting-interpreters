struct Line {
  int pos;
  int length;
  int capacity;
  char *start;
};

void initLine(struct Line *line);
void insertLine(struct Line *line, char c);
void removeLine(struct Line *line);
void setPosLine(struct Line *line, int index);
void freeLine(struct Line *line);
