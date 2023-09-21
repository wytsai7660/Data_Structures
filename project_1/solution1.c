#include <stdio.h>

// typedef struct {
//   int x, y, from_dir;
//   int *ans;
// } Point;

typedef struct __Node__ {
  int x, y, from_dir;
  int *ans;
  struct __Node__ *next;
} Node;

typedef struct {
  Node *head, *tail;
  int size;
} Queue;

Queue *new_queue() {
  Queue *tmp_q = malloc(sizeof(Queue));
  *tmp_q = (Queue){NULL, NULL, 0};
  return tmp_q;
}

void queue_push(Queue *queue, Point point) {
  Node *tmp_node = malloc(sizeof(Node));
  *tmp_node = (Node){point, NULL};
  if (queue->size == 0) {
    queue->head = queue->tail = tmp_node;
  } else {
    queue->tail->next = tmp_node;
    queue->tail = tmp_node;
  }
  queue->size++;
}

Point queue_pop(Queue *queue) {
  if (queue->size == 0) {
    printf("queue: attempt to pop an element from an empty queue\n");
    exit(1);
  }
  if (queue->size == 1) {  // head == tail
    free(queue->head);
    queue->head = queue->tail = NULL;
  }
  Point tmp_Point free(queue->head);

  // Point tmp_point = queue->head->data;
  // Node *tmp_node = queue->head;
  // queue->head = queue->head->next;
  // free(tmp_node);
  queue->size--;
  // return tmp_point;
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
  Point ball_1 = (Point){tmp_1, tmp_2, -1};  // set the ball

  scanf("%d%d", &tmp_1, &tmp_2);
  Point ball_2 = (Point){tmp_1, tmp_2, -1};  // set the ball

  scanf("%d%d", &tmp_1, &tmp_2);
  maze[tmp_2][tmp_1] = 'D';  // set the destination

  scanf("%d%d", &tmp_1, &tmp_2);
  maze[tmp_2][tmp_1] = 'D';  // set the destination

  Queue *q = new_queue(n * n);
  queue_push(q, ball_1);

  int x, y;
  while (q->size) {  // BFS for the first ball (two destinations are ok)
    Point p = queue_pop(q);
    for (int dir = 0; dir < 4; dir++) {            // try to move in each directionß
      if (dir != 3 - p.from_dir &&                 // not going back
          (x = p.x + direction[dir][0]) != '1' &&  //
          (y = p.y + direction[dir][1]) != '1') {
        // int x = p.x + direction[dir][0], y = p.y + direction[dir][1];
        // if (x >= 0 && x < n && y >= 0 && y < n && maze[y][x] != 'X') {
        //   maze[y][x] = 'X';
        //   queue_push(q, (Point){x, y, dir});
        // }
      }
    }
  }
}
