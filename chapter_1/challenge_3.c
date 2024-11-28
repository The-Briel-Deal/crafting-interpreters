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

struct Node *find_node(struct Node *start, int target) {
  struct Node *head = start;
  // Search forwards
  while (head) {
    if (head->index == target)
      return head;
    head = head->next;
  }

  // Search backwards
  head = start;
  while (head) {
    if (head->index == target)
      return head;
    head = head->prev;
  }
  return NULL;
}

void find_and_insert_node(struct Node *head, int find_index, int new_index) {
  struct Node *found = find_node(head, find_index);
  struct Node *prev = found;
  struct Node *next = found->next;

  struct Node *new = malloc(sizeof(struct Node));
  new->index = new_index;
  new->prev = prev;
  new->next = next;

  if (prev)
    prev->next = new;
  if (next)
    next->prev = new;
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

  // Find a node
  head = start;
  struct Node *third_node = head->next->next;
  struct Node *found_node = find_node(head, 2);
  assert(third_node == found_node);
  printf("Address of third node %p\n", third_node);
  printf("Address of found node %p\n", found_node);

  // Insert a node
  find_and_insert_node(head, 1, 8);
  struct Node* new_node = head->next->next;
  assert(new_node->index == 8);

  // Pop a node
  head = start;
  pop_node(head);

  assert(head->index == 0);
  printf("head index is %i\n", head->index);
  head = head->next;

  assert(head->index == 1);
  printf("head index is %i\n", head->index);
  head = head->next;

  assert(head->index == 8);
  printf("head index is %i\n", head->index);
  head = head->next;

  assert(head->index == 2);
  printf("head index is %i\n", head->index);
  head = head->next;

  assert(head == NULL);
}
