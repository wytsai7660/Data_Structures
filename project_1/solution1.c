// i have no idea what i'm writing
// this file is a piece of s**t
// why am i here

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  //tmp

// #define int long long
// #define unsigned unsigned long long
// #define i32 int

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
  int ball_2_x, ball_2_y;
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
  Queue *tmp_queue = malloc(sizeof(Queue));
  *tmp_queue = (Queue){NULL, NULL, 0};
  return tmp_queue;
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
  // printf("(-) popping (%d, %d, %d)\n", queue->head->data.x, queue->head->data.y, queue->head->data.from_dir);
  Node tmp_node = *queue->head;
  free(queue->head);
  queue->head = tmp_node.next;
  if (queue->size == 1) queue->tail = NULL;
  queue->size--;
  return tmp_node.data;
}

void queue_print(Queue *queue) {
  // Node *tmp_node = queue->head;
  // printf("queue head: %p, queue tail: %p, queue size: %d\n", queue->head, queue->tail, queue->size);
  // while (tmp_node != NULL) {
  //   printf("(%d, %d, %d)\n", tmp_node->data.x, tmp_node->data.y, tmp_node->data.from_dir);
  //   tmp_node = tmp_node->next;
  // }
}

void queue_clear(Queue *queue) {
  // printf("start freeing queue\n");
  while (queue->size) free(queue_pop(queue).ans);
  free(queue);
  // printf("queue freed\n");
}

int main() {
  // input area starts here
  int n;  // row & col
  scanf("%d\n", &n);

  char **maze = malloc((unsigned)n * sizeof(char *));
  for (int row = 0; row < n; row++) maze[row] = malloc((unsigned)n * sizeof(char));
  for (int y = n - 1; y >= 0; y--)
    for (int x = 0; x < n; x++) scanf("%c ", &maze[y][x]);  // read the maze

  Point ball_1 = {.from_dir = -1, .ans = NULL, .ans_len = 0}, ball_2 = ball_1;
  scanf("%d%d%d%d", &ball_1.x, &ball_1.y, &ball_2.x, &ball_2.y);
  ball_1.ball_2_x = ball_2.x, ball_1.ball_2_y = ball_2.y;

  int tmp_1, tmp_2;  // used to temporarily store the input

  scanf("%d%d", &tmp_1, &tmp_2);
  maze[tmp_2][tmp_1] = 'D';  // set the destination

  scanf("%d%d", &tmp_1, &tmp_2);
  maze[tmp_2][tmp_1] = 'D';  // set the destination
  // input area ends here

  Queue *queue = new_queue();
  queue_push(queue, ball_1);

  // printf("(+) pushing (1, 1, -1) ans: \n");

  int *ans, ans_len;
  int x, y;
  bool continue_loop = true;
  while (continue_loop) {  // BFS for the first ball (two destinations are ok)
    const Point working_point = queue_pop(queue);
    for (int dir = 0; dir < 4; dir++) {                   // try to move in each direction
      if (dir == (2 ^ working_point.from_dir)) continue;  // avoid going back
      x = working_point.x + direction[dir][0], y = working_point.y + direction[dir][1];
      if (x < 0 || x >= n || y < 0 || y >= n || maze[y][x] == '1') continue;  // out of range or wall

      // printf("(%d, %d) trying to move %d to (%d, %d)\n", working_point.x, working_point.y, dir, x, y);

      if (maze[y][x] == 'D') {
        // printf("stop!\n");
        // sleep(100);
        continue_loop = false;

        ans_len = working_point.ans_len + 1;
        ans = malloc((unsigned)ans_len * sizeof(int));
        if (working_point.ans_len) memcpy(ans, working_point.ans, (unsigned)working_point.ans_len * sizeof(int));
        ans[ans_len - 1] = dir;

        ball_2.x = working_point.ball_2_x + direction[dir][0], ball_2.y = working_point.ball_2_y + direction[dir][1];
        if (maze[ball_2.y][ball_2.x] == '1') ball_2.x = working_point.ball_2_x, ball_2.y = working_point.ball_2_y;

        maze[y][x] = '0';  // mark the destination as a normal path

        break;
      }

      Point tmp_point = (Point){x, y, dir, malloc(((unsigned)working_point.ans_len + 1) * sizeof(int)), working_point.ans_len + 1, working_point.ball_2_x + direction[dir][0], working_point.ball_2_y + direction[dir][1]};

      // TODO: deal with the case when the second ball falls into the destination earlier than the first ball
      if (maze[tmp_point.ball_2_y][tmp_point.ball_2_x] == '1') tmp_point.ball_2_x = working_point.ball_2_x, tmp_point.ball_2_y = working_point.ball_2_y;

      if (working_point.ans_len) memcpy(tmp_point.ans, working_point.ans, (unsigned)working_point.ans_len * sizeof(int));

      tmp_point.ans[tmp_point.ans_len - 1] = dir;

      queue_push(queue, tmp_point);

      // printf("(+) pushing (%d, %d, %d) ans: ", tmp_point.x, tmp_point.y, tmp_point.from_dir);
      // for (int i = 0; i < tmp_point.ans_len; i++) printf("%d", tmp_point.ans[i]);
      // printf("\n");
      // sleep(1);
    }
    free(working_point.ans);
  }

  // debug log
  // printf("ans_len: %d\n", ans_len);
  // for (int i = 0; i < ans_len; i++) printf("%d", ans[i]);
  // printf("\n");
  //

  queue_clear(queue);
  queue = new_queue();
  queue_push(queue, ball_2);

  continue_loop = maze[ball_2.y][ball_2.x] == 'D' ? false : true;
  while (continue_loop) {  // BFS for the second ball (only one destination)
    const Point working_point = queue_pop(queue);
    for (int dir = 0; dir < 4; dir++) {                   // try to move in each direction
      if (dir == (2 ^ working_point.from_dir)) continue;  // avoid going back
      x = working_point.x + direction[dir][0], y = working_point.y + direction[dir][1];
      if (x < 0 || x >= n || y < 0 || y >= n || maze[y][x] == '1') continue;  // out of range or wall

      // printf("(%d, %d) trying to move %d to (%d, %d)\n", working_point.x, working_point.y, dir, x, y);

      if (maze[y][x] == 'D') {
        // printf("stop!\n");
        // sleep(100);
        continue_loop = false;

        ans = realloc(ans, (unsigned)(ans_len + working_point.ans_len + 1) * sizeof(int));
        // ans_len += working_point.ans_len + 1;
        // ans = malloc((unsigned)ans_len * sizeof(int));
        if (working_point.ans_len) memcpy(ans + ans_len, working_point.ans, (unsigned)working_point.ans_len * sizeof(int));
        ans_len += working_point.ans_len + 1;
        ans[ans_len - 1] = dir;

        // if (working_point.ans_len) memcpy(ans, working_point.ans, (unsigned)working_point.ans_len * sizeof(int));
        // ans[ans_len - 1] = dir;

        break;
      }

      Point tmp_point = (Point){x, y, dir, malloc(((unsigned)working_point.ans_len + 1) * sizeof(int)), working_point.ans_len + 1};

      if (working_point.ans_len) memcpy(tmp_point.ans, working_point.ans, (unsigned)working_point.ans_len * sizeof(int));

      tmp_point.ans[tmp_point.ans_len - 1] = dir;

      queue_push(queue, tmp_point);

      // printf("(+) pushing (%d, %d, %d) ans: ", tmp_point.x, tmp_point.y, tmp_point.from_dir);
      // for (int i = 0; i < tmp_point.ans_len; i++) printf("%d", tmp_point.ans[i]);
      // printf("\n");
      // sleep(1);
    }
    free(working_point.ans);
  }

  // debug log
  // printf("ans_len: %d\n", ans_len);
  for (int i = 0; i < ans_len; i++) printf("%d", ans[i]);
  // printf("\n");
  //

  free(ans);
  queue_clear(queue);
  for (int row = 0; row < n; row++) free(maze[row]);
  free(maze);
}
