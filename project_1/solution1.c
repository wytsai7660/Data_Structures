#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

// y ^
//   |             up(0)
//   |              |
//   |   left(3) -- + -- right(1)
//   |              |
//   |            down(2)
//   |
//   + ----------------------> x

typedef struct {
  int x, y, from_dir;
} point;

typedef struct {
  point *data;
  unsigned head, size, capacity;
} queue;  // circular queue with fixed capacity (the should be no more than capacity elements in the queue)

queue *new_queue(unsigned capacity) {
  queue *q = malloc(sizeof(queue));
  *q = (queue){malloc(capacity * sizeof(point)), 0, 0, capacity};
  return q;
}

point queue_front(queue *q) { return q->size > 0 ? q->data[q->head] : (point){-1, -1, -1}; }

point queue_pop(queue *q) {
  if (q->head >= q->capacity) q->head = 0;
  q->size--;
  return q->data[q->head++];
}

void queue_push(queue *q, int x, int y, int from_dir) {
  if (q->size >= q->capacity) queue_pop(q);
  q->data[(q->head + (q->size++)) % q->capacity] = (point){x, y, from_dir};
}

void queue_clear(queue *q) {
  free(q->data);
  free(q);
}

int main() {
  unsigned n;  // row & col
  scanf("%d", &n);
  char **maze = malloc(n * sizeof(char *));
  for (unsigned y = 0; y < n; y++) maze[y] = malloc(n * sizeof(char));
  int tmp, tmp2;  // used to temporarily store the input
  for (unsigned y = n - 1; y >= 0; y--)
    for (unsigned x = 0; x < n; x++)
      while (!isspace(tmp = getchar())) maze[y][x] = (char)tmp;  // as the assignment didn't specify the input format for the matrix, we need to consider the case that the input may contain spaces

  scanf("%d%d", &tmp, &tmp2);
  maze[tmp2][tmp] = 'B';  // set the ball

  scanf("%d%d", &tmp, &tmp2);
  maze[tmp2][tmp] = 'B';  // set the ball

  scanf("%d%d", &tmp, &tmp2);
  maze[tmp2][tmp] = 'D';  // set the destination

  scanf("%d%d", &tmp, &tmp2);
  maze[tmp2][tmp] = 'D';  // set the destination

  queue *q = new_queue(n * n);

  // BFS for the first ball (two destinations are ok)
}