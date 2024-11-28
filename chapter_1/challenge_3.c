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

  next->index = head->index + 1;

  head->next = next;
}

int main(int argc, char **argv) {
  struct Node *head = init_linked_list();
  push_node(head);
  push_node(head);
  push_node(head);

  assert(head->index == 0);
  printf("head index is %i\n", head->index);
  head = head->next;

  assert(head->index == 1);
  printf("head index is %i\n", head->index);
  head = head->next;
  assert(head->index == 2);
  printf("head index is %i\n", head->index);
  head = head->next;
  assert(head->index == 3);
  printf("head index is %i\n", head->index);
  head = head->next;

  assert(head == NULL);
}
