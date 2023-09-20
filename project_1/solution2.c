#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  unsigned x, y;
} ball;

int main() {
  // input starts here
  unsigned n;
  scanf("%u\n", &n);

  bool **maze = malloc(n * sizeof(bool *));
  for (unsigned row = 0; row < n; row++) maze[row] = malloc(n * sizeof(bool));
  int tmp;  // since there is no format specifier for bool in C, we use int to temporarily store the input
  for (unsigned y = n - 1; y <= n; y--)
    for (unsigned x = 0; x < n; x++) {
      while (isspace(tmp = getchar()))
        ;
      maze[y][x] = tmp == '1';
    }

  ball ball_1, ball_2;
  scanf("%d%d%d%d", &ball_1.x, &ball_1.y, &ball_2.x, &ball_2.y);
}