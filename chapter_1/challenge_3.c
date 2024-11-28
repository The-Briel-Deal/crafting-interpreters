#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct Node {
  struct Node *next;
  struct Node *prev;
  int index;
};

struct Node *init_linked_list(int index) {
  struct Node *head = malloc(sizeof(struct Node));
  head->index = index;
  head->next = NULL;
  head->prev = NULL;
  return head;
}

void push_node(struct Node *head, int index) {
  struct Node *next = malloc(sizeof(struct Node));
  next->next = NULL;
  next->index = index;
  while (head->next) {
    head = head->next;
  }
  assert(head->next == NULL);

  next->prev = head;
  head->next = next;
}

void pop_node(struct Node *head) {
  struct Node *prev = NULL;
  while (head->next) {
    prev = head;
    head = head->next;
  }
  free(head);
  prev->next = NULL;
}

int main(int argc, char **argv) {
  struct Node *head = init_linked_list(0);
  struct Node *start = head;
  push_node(head, 1);
  push_node(head, 2);
  push_node(head, 3);

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
  head = head->prev; // Start going backwards

  assert(head->index == 2);
  printf("head index is %i\n", head->index);
  head = head->prev;

  assert(head->index == 1);
  printf("head index is %i\n", head->index);
  head = head->prev;

  assert(head->index == 0);
  printf("head index is %i\n", head->index);
  head = head->prev;

  assert(head == NULL);

  head = start;
  pop_node(head);

  assert(head->index == 0);
  printf("head index is %i\n", head->index);
  head = head->next;

  assert(head->index == 1);
  printf("head index is %i\n", head->index);
  head = head->next;

  assert(head->index == 2);
  printf("head index is %i\n", head->index);
  head = head->next;

  assert(head == NULL);
}
