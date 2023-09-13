#include <stdio.h>
#include <stdlib.h>

#include "maze_generator.c"

void test(int **mat, unsigned n) {
  for (unsigned i = 0; i < n; i++)
    for (unsigned j = 0; j < n; j++) mat[i][j] = 99;
}

void print(int **mat, unsigned n) {
  for (unsigned i = 0; i < n; i++) {
    for (unsigned j = 0; j < n; j++) printf("%d ", mat[i][j]);
    printf("\n");
  }
}

int main() {
  const unsigned n = 10;  // row & col

  int **maze = malloc(n * sizeof(int *));
  for (unsigned row = 0; row < n; row++) maze[row] = malloc(n * sizeof(int));

  // make_IntTriple(rand_between(0, n - 2) | 1, rand_between(0, n - 2) | 1, rand_between(0, 3));

  print(maze, n);

  test(maze, n);

  print(maze, n);

  for (unsigned row = 0; row < n; row++) free(maze[row]);
  free(maze);
}