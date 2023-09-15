#include <stdio.h>

typedef struct {
  int *data;
  int head, size, capacity;
} queue;  // circular queue with fixed capacity (the should be no more than capacity elements in the queue)

queue *new_queue(int capacity) {
  queue *q = malloc(sizeof(queue));
  *q = (queue){malloc(capacity * sizeof(int)), 0, 0, capacity};
  return q;
}

int queue_front(queue *q) { return q->size > 0 ? q->data[q->head] : -1; }

int queue_pop(queue *q) {
  if (q->head >= q->capacity) q->head = 0;
  q->size--;
  return q->data[q->head++];
}

void queue_push(queue *q, int num) {
  if (q->size >= q->capacity) queue_pop(q);
  q->data[(q->head + (q->size++)) % q->capacity] = num;
}

void queue_clear(queue *q) {
  free(q->data);
  free(q);
}

int main() {
  int n;
  scanf("%d", &n);
}