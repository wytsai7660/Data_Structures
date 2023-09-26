// the second solution to the project
// or not?
// i can't even optimize the first solution
// pethetic

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // for sleep()

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
  int x, y;
} coordinate;

// typedef struct {
//   bool data[4];
// } simple_set;

// simple_set new_simple_set(void) { return (simple_set){false, false, false, false}; }

// void simple_set_insert(simple_set set, int dir) { set.data[dir] = true; }

// bool simple_set_find(simple_set set, int dir) { return set.data[dir]; }

typedef struct {
  coordinate ball_1, ball_2;
  bool des_1_active, des_2_active;
  int *ans, ans_len;
  // simple_set not_valid;
} Status;

typedef struct Node {
  Status data;
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

void queue_push(Queue *queue, Status status) {
  Node *tmp_node = malloc(sizeof(Node));
  *tmp_node = (Node){status, NULL};
  if (queue->size == 0) {
    queue->head = queue->tail = tmp_node;
  } else {
    queue->tail->next = tmp_node;
    queue->tail = tmp_node;
  }
  queue->size++;
}

Status queue_pop(Queue *queue) {  // pop out & return the front element
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
  int n;  // n: row & col, tmp: used to temporarily store the input
  scanf("%d", &n);

  char **maze = malloc((unsigned)(n + 2) * sizeof(char *));
  for (int row = 0; row < n + 2; row++) maze[row] = malloc((unsigned)(n + 2) * sizeof(char));

  for (int y = n - 1; y >= 0; y--)  // read the maze
    for (int x = 0; x < n; x++)
      while (isspace(maze[y][x] = (char)getchar())) continue;

  for (int y = n - 1; y < n + 2; y++)  // ball trap for finished balls
    for (int x = n - 1; x < n + 2; x++) maze[y][x] = '1';

  Status status = {.des_1_active = true, .des_2_active = true, .ans = NULL, .ans_len = 0};  // initialize status, later used to try to emulate the movement

  scanf("%d%d%d%d", &status.ball_1.x, &status.ball_1.y, &status.ball_2.x, &status.ball_2.y);  //

  coordinate des_1, des_2;
  scanf("%d%d%d%d", &des_1.x, &des_1.y, &des_2.x, &des_2.y);  //
  maze[des_1.y][des_1.x] = 'D', maze[des_2.y][des_2.x] = 'D';

  // input area ends here
  // printf("read ok\n");

  Queue *queue = new_queue();
  queue_push(queue, status);

  // printf("(+) pushing (1, 1, -1) ans: \n");

  // int *ans, ans_len;

  int move_back;
  bool continue_loop = true;

  while (continue_loop) {  // BFS at the same time
    const Status working_status = queue_pop(queue);
    for (int dir = 0; dir < 4; dir++) {  // try to move in each direction

      // status = (Status){(coordinate){working_status.ball_1.x + direction[dir][0], working_status.ball_1.y + direction[dir][1]},
      //                   (coordinate){working_status.ball_2.x + direction[dir][0], working_status.ball_2.y + direction[dir][1]},  //
      //                   working_status.des_1_active,
      //                   working_status.des_2_active,
      //                   malloc(((unsigned)working_status.ans_len + 1) * sizeof(int)),
      //                   working_status.ans_len + 1};  // try to perform the movement

      // status = (Status){(coordinate){working_status.ball_1.x + direction[dir][0], working_status.ball_1.y + direction[dir][1]}, (coordinate){working_status.ball_2.x + direction[dir][0], working_status.ball_2.y + direction[dir][1]},  //
      //                   working_status.des_1_active, working_status.des_2_active,                                                                                                                                                        //
      //                   .ans_len = working_status.ans_len + 1};                                                                                                                                                                          // try to perform the movement

      status.ball_1 = (coordinate){working_status.ball_1.x + direction[dir][0], working_status.ball_1.y + direction[dir][1]};
      status.ball_2 = (coordinate){working_status.ball_2.x + direction[dir][0], working_status.ball_2.y + direction[dir][1]};
      status.des_1_active = working_status.des_1_active;  // TODO: maybe not necessary?
      status.des_2_active = working_status.des_2_active;
      status.ans_len = working_status.ans_len + 1;

      move_back = 0;
      if (maze[status.ball_1.y][status.ball_1.x] == '1') status.ball_1.x = working_status.ball_1.x, status.ball_1.y = working_status.ball_1.y, move_back++;  // invalid (wall), reset the ball's position
      if (maze[status.ball_2.y][status.ball_2.x] == '1') status.ball_2.x = working_status.ball_2.x, status.ball_2.y = working_status.ball_2.y, move_back++;  // invalid (wall), reset the ball's position
      if (move_back == 2) {                                                                                                                                  // not a meaningful movement, skip
        // free(status.ans);
        continue;
      }

      if (maze[status.ball_1.y][status.ball_1.x] == 'D') {  // check if ball_1 has reached the destination
        if (status.des_1_active && status.ball_1.x == des_1.x && status.ball_1.y == des_1.y) status.des_1_active = false, status.ball_1.x = status.ball_1.y = n, maze[status.ball_1.y][status.ball_1.x] = '0';
        else if (status.des_2_active && status.ball_1.x == des_2.x && status.ball_1.y == des_2.y) status.des_2_active = false, status.ball_1.x = status.ball_1.y = n, maze[status.ball_1.y][status.ball_1.x] = '0';
      }
      if (maze[status.ball_2.y][status.ball_2.x] == 'D') {  // check if ball_2 has reached the destination
        if (status.des_1_active && status.ball_2.x == des_1.x && status.ball_2.y == des_1.y) status.des_1_active = false, status.ball_2.x = status.ball_2.y = n, maze[status.ball_2.y][status.ball_2.x] = '0';
        else if (status.des_2_active && status.ball_2.x == des_2.x && status.ball_2.y == des_2.y) status.des_2_active = false, status.ball_2.x = status.ball_2.y = n, maze[status.ball_2.y][status.ball_2.x] = '0';
      }

      if (!(status.des_1_active || status.des_2_active)) {
        // printf("Finished!\n");
        // if (status.ans == NULL) printf("NULL\n");
        // else
        for (int i = 0; i < working_status.ans_len; i++) printf("%d", working_status.ans[i]);
        printf("%d\n", dir);
        continue_loop = false;
        break;
      }

      status.ans = malloc((unsigned)status.ans_len * sizeof(int));
      if (working_status.ans != NULL) memcpy(status.ans, working_status.ans, (unsigned)working_status.ans_len * sizeof(int));
      status.ans[status.ans_len - 1] = dir;  // record the movement

      queue_push(queue, status);

      printf("(+) pushing (%d, %d), (%d, %d), ans_len: %d,\tans: ", status.ball_1.x, status.ball_1.y, status.ball_2.x, status.ball_1.y, status.ans_len);
      for (int i = 0; i < status.ans_len; i++) printf("%d", status.ans[i]);
      printf("\n");
      // sleep(1);
    }
    if (working_status.ans != NULL) free(working_status.ans);  // TODO: maybe not necessary?
  }

  // debug log
  // printf("ans_len: %d\n", ans_len);
  // for (int i = 0; i < ans_len; i++) printf("%d", ans[i]);
  // printf("\n");
  //

  // debug log
  // printf("ans_len: %d\n", ans_len);
  // for (int i = 0; i < ans_len; i++) printf("%d", ans[i]);
  // printf("\n");
  //

  // free(ans);
  queue_clear(queue);
  for (int row = 0; row < n + 2; row++) free(maze[row]);
  free(maze);  //
}
