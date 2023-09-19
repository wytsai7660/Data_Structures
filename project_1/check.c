#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *dir_name[4] = {"up", "right", "down", "left"};

const int direction[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

const char *const symbol[] = {" ", "@", "\e[31mB\e[0m", "\e[33mD\e[0m", "\e[32mD\e[0m"};  // space, wall, ball, destination, destination with ball

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
  char **maze = malloc((unsigned)(n + 3) * sizeof(char *));
  for (int row = 0; row < n + 3; row++) maze[row] = memset(malloc((unsigned)(n + 3) * sizeof(char)), '1', (unsigned)(n + 3));

  for (int y = n - 1; y >= 0; y--) {
    for (int x = 0; x < n; x++) scanf("%c ", &maze[y][x]);
  }

  int ball_1[2], ball_2[2];  // ball[x/y]

  int a, b;
  scanf("%d%d", &ball_1[0], &ball_1[1]);
  scanf("%d%d", &ball_2[0], &ball_2[1]);

  scanf("%d%d", &a, &b);
  maze[b][a] = '3';
  scanf("%d%d", &a, &b);
  maze[b][a] = '3';

  print_maze(maze, n, ball_1, ball_2);

  int dir;
  while (scanf("%1d", &dir) != EOF) {
    printf("\nmove %s\n", dir_name[dir]);
    if (maze[ball_1[1] + direction[dir][1]][ball_1[0] + direction[dir][0]] != '1') {
      ball_1[0] += direction[dir][0], ball_1[1] += direction[dir][1];
      if (maze[ball_1[1]][ball_1[0]] == '3') maze[ball_1[1]][ball_1[0]] = '4', ball_1[0] = ball_1[1] = n;
    }
    if (maze[ball_2[1] + direction[dir][1]][ball_2[0] + direction[dir][0]] != '1') {
      ball_2[0] += direction[dir][0], ball_2[1] += direction[dir][1];
      if (maze[ball_2[1]][ball_2[0]] == '3') maze[ball_2[1]][ball_2[0]] = '4', ball_2[0] = ball_2[1] = n;
    }
    print_maze(maze, n, ball_1, ball_2);
  }

  for (int row = 0; row < n + 3; row++) free(maze[row]);
  free(maze);
}