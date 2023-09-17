#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #define MAX_PATHES_NUM (N * N - N - N + 1) / 2
// unsigned pathes[MAX_PATHES_NUM][2];  // save for later use
// unsigned pathes_num = 0;

const int direction[4][2] = {{0, 1}, {1, 0}, {0, 1}, {-1, 0}};

const char *const symbol[] = {" ", "@", "\e[31mB\e[0m", "\e[33mD\e[0m"};  // space, wall, ball, destination

void print_maze(char **maze, int n, int *ball_1, int *ball_2) {
  for (int y = n - 1; y >= 0; y--) {
    for (int x = 0; x < n; x++) {
      if (x == ball_1[0] && y == ball_1[1]) printf("%s ", symbol[2]);
      else if (x == ball_2[0] && y == ball_2[1]) printf("%s ", symbol[2]);
      else printf("%s ", symbol[maze[y][x] - '0']);
    }
    putchar('\n');
  }
}

int main() {
  int n;
  scanf("%d\n", &n);
  char **maze = malloc((unsigned)n * sizeof(char *));
  for (int row = 0; row < n; row++) maze[row] = memset(malloc((unsigned)n * sizeof(char)), 1, (unsigned)n);

  for (int y = n - 1; y >= 0; y--) {
    for (int x = 0; x < n; x++) scanf("%c ", &maze[y][x]);
  }

  int ball_1[2], ball_2[2];  // ball[ball_num][x/y]

  int a, b;
  scanf("%d%d", &ball_1[0], &ball_1[1]);
  scanf("%d%d", &ball_2[0], &ball_2[1]);

  scanf("%d%d", &a, &b);
  maze[b][a] = '3';
  scanf("%d%d", &a, &b);
  maze[b][a] = '3';

  print_maze(maze, n, ball_1, ball_2);

  int dir;
  while (scanf("%d", &dir) != EOF) {
    maze[ball_1[1] + direction[dir][1]][ball_1[0] + direction[dir][0]];
    print_maze(maze, n, ball_1, ball_2);
  }

  for (int row = 0; row < n; row++) free(maze[row]);
  free(maze);
}