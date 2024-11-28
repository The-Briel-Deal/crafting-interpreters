#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
  struct Node *next;
  int index;
};

struct Node *init_linked_list() {
  struct Node *head = malloc(sizeof(struct Node));
  head->index = 0;
  head->next = NULL;
  return head;
}

void push_node(struct Node *head) {
  struct Node *next = malloc(sizeof(struct Node));
  next->index = -1;
  next->next = NULL;
  while (head->next != NULL) {
    head = head->next;
  }
  assert(head->next == NULL);
  head->next = next;
}

int main(int argc, char **argv) {
  struct Node head = {.next = NULL, .index = 1};
  struct Node next_node = {.next = NULL, .index = 2};
  head.next = &next_node;

  printf("head index is %i\n", head.index);
  printf("head.next index is %i\n", head.next->index);
}
