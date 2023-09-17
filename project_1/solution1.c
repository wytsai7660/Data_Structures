// solution1: BFS two times, one for each ball

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
const int direction[4][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

typedef struct {
  int x, y, from_dir;
} point;

typedef struct {
  point *data;
  int head, size, capacity;
} queue;  // circular queue with fixed capacity (the should be no more than capacity elements in the queue)

queue *new_queue(int capacity) {
  queue *q = malloc(sizeof(queue));
  if (q == NULL) {
    printf("queue: failed to allocate memory for queue (tried to allocate %u bytes)\n", sizeof(queue));
    exit(1);
  }
  *q = (queue){malloc(capacity * sizeof(point)), 0, 0, capacity};
  if (q->data == NULL) {
    printf("queue: failed to allocate memory for queue.data (tried to allocate %u bytes)\n", capacity * sizeof(point));
    exit(1);
  }
  return q;
}

point queue_front(queue *q) {
  if (q->size == 0) {
    printf("queue: attempt to get front element of an empty queue\n");
    exit(1);
  }
  return q->data[q->head];
}

point queue_pop(queue *q) {  // FIXME //TODO: rewrite this function
  if (q->head >= q->capacity) q->head = 0;
  q->size--;
  return q->data[q->head++];
}

void queue_push(queue *q, point p) {
  if (q->size == q->capacity) {
    printf("queue: attempt to push an element to a full queue, popping the front element to resolve\n");
    queue_pop(q);
  }
  q->data[(q->head + (q->size++)) % q->capacity] = p;
}

void queue_clear(queue *q) {
  free(q->data);
  free(q);
}

int main() {
  int n;  // row & col
  scanf("%d", &n);

  char **maze = malloc(n * sizeof(char *));
  for (int y = 0; y < n; y++) maze[y] = malloc(n * sizeof(char));
  for (int y = n - 1; y >= 0; y--)
    for (int x = 0; x < n; x++) scanf("%c ", &maze[y][x]);  // read the maze

  int tmp_1, tmp_2;  // used to temporarily store the input
  scanf("%d%d", &tmp_1, &tmp_2);
  point ball_1 = (point){tmp_1, tmp_2, -1};  // set the ball

  scanf("%d%d", &tmp_1, &tmp_2);
  point ball_2 = (point){tmp_1, tmp_2, -1};  // set the ball

  scanf("%d%d", &tmp_1, &tmp_2);
  maze[tmp_2][tmp_1] = 'D';  // set the destination

  scanf("%d%d", &tmp_1, &tmp_2);
  maze[tmp_2][tmp_1] = 'D';  // set the destination

  queue *q = new_queue(n * n);
  queue_push(q, ball_1);

  int x, y;
  while (q->size) {  // BFS for the first ball (two destinations are ok)
    point p = queue_pop(q);
    for (int dir = 0; dir < 4; dir++) {            // try to move in each directionß
      if (dir != 3 - p.from_dir &&                 // not going back
          (x = p.x + direction[dir][0]) != '1' &&  //
          (y = p.y + direction[dir][1]) != '1') {
        // int x = p.x + direction[dir][0], y = p.y + direction[dir][1];
        // if (x >= 0 && x < n && y >= 0 && y < n && maze[y][x] != 'X') {
        //   maze[y][x] = 'X';
        //   queue_push(q, (point){x, y, dir});
        // }
      }
    }
  }
}