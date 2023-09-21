// i have no idea what i'm writing
// this file is a piece of s**t
// why am i here

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// y ^
//   |             up(0)
//   |              |
//   |   left(3) -- + -- right(1)
//   |              |
//   |            down(2)
//   |
//   + ----------------------> x
const int direction[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};  // dir_num -> dir[x/y]

typedef struct {
  int x, y, from_dir, *ans, ans_len;
} Point;

typedef struct Node {
  Point data;
  struct Node *next;
} Node;

typedef struct {
  Node *head, *tail;
  int size;
} Queue;  // for this task there is no need for implementing standalone "front" function

Queue *new_queue(void) {
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

Point queue_pop(Queue *queue) {  // pop out & return the front element
  if (queue->size == 0) {
    printf("queue: attempt to pop an element from an empty queue\n");
    exit(1);
  }
  if (queue->size == 1) {  // head == tail
    free(queue->head);
    queue->head = queue->tail = NULL;
  }
  Node tmp_node = *queue->head;
  free(queue->head);
  queue->head = tmp_node.next;
  queue->size--;

  return tmp_node.data;
}

void queue_free(Queue *queue) {
  while (queue->size) queue_pop(queue);
  free(queue);
}

int main() {
  // input area starts here
  int n;  // row & col
  scanf("%d", &n);

  char **maze = malloc(n * sizeof(char *));
  for (int y = 0; y < n; y++) maze[y] = malloc(n * sizeof(char));
  for (int y = n - 1; y >= 0; y--)
    for (int x = 0; x < n; x++) scanf("%c ", &maze[y][x]);  // read the maze

  int tmp_1, tmp_2;  // used to temporarily store the input
  scanf("%d%d", &tmp_1, &tmp_2);
  Point ball_1 = (Point){tmp_1, tmp_2, -1, NULL, 0};  // set the ball

  scanf("%d%d", &tmp_1, &tmp_2);
  Point ball_2 = (Point){tmp_1, tmp_2, -1, NULL, 0};  // set the ball

  scanf("%d%d", &tmp_1, &tmp_2);
  maze[tmp_2][tmp_1] = 'D';  // set the destination

  scanf("%d%d", &tmp_1, &tmp_2);
  maze[tmp_2][tmp_1] = 'D';  // set the destination

  Queue *queue = new_queue();
  queue_push(queue, ball_1);
  // input area ends here

  int *ans, ans_len;
  int x, y;
  bool continue_loop = true;
  while (continue_loop) {  // BFS for the first ball (two destinations are ok)
    const Point working_point = queue_pop(queue);
    for (int dir = 0; dir < 4; dir++) {                 // try to move in each direction
      if (dir == 4 - working_point.from_dir) continue;  // avoid going back
      x = working_point.x + direction[dir][0], y = working_point.y + direction[dir][1];
      if (x < 0 || x >= n || y < 0 || y >= n || maze[y][x] == '1') continue;  // out of range or wall

      if (maze[y][x] == 'D') {
        continue_loop = false, ans = working_point.ans, ans_len = working_point.ans_len;
        break;
      }
      Point tmp_point;
      tmp_point.x = x, tmp_point.y = y, tmp_point.from_dir = dir;
      tmp_point.ans_len = working_point.ans_len + 1;
      tmp_point.ans = memcpy(malloc(tmp_point.ans_len * sizeof(int)), working_point.ans, tmp_point.ans_len * sizeof(int));
      tmp_point.ans[tmp_point.ans_len - 1] = dir;

      queue_push(queue, tmp_point);
    }
  }
}
