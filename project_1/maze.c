#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 45  // row & col

#define MAX_PATHES_NUM (N * N - N - N + 1) / 2
unsigned pathes[MAX_PATHES_NUM][2];  // save for later use
unsigned pathes_num = 0;

// get a random number between [l, r] 
// note: should not use it repeatedly (potential harm to performance)
unsigned rand_between(unsigned l, unsigned r) { return (unsigned)rand() % (r - l + 1) + l; }

// (row, col)
//      1            (-, 0)
//   2     0   (0, -)      (0, +)
//      3            (+, 0)
const int direction[4][2] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};  // directions[dir][delta_(row: 0/ col: 1)]

// generate function
void generate(bool **maze, unsigned row, unsigned col, unsigned from_dir) {
  if (pathes_num >= MAX_PATHES_NUM) return;  // stop when finished generating (avoid unnecessary calculation)

  unsigned dir = rand_between(from_dir + 3, from_dir + 5) % 4, first_row, first_col, second_row, second_col;  // choose a new direction randomly from 0 to 3, but avoid choosing the opposite direction
                                                                                                              // second_row & second_col may go out of bound (negative), but don't worry, overflow of unsigned int is not undefined behavior

  for (int i = 0; i < 3; i++) {                                                                                            // try at most three directions
    second_row = (unsigned)((int)row + 2 * direction[dir][0]), second_col = (unsigned)((int)col + 2 * direction[dir][1]);  // next position

    if (second_row < N && second_col < N && maze[first_row = (unsigned)((int)row + direction[dir][0])][first_col = (unsigned)((int)col + direction[dir][1])] && maze[second_row][second_col]) {  // there's no need to check if the coordinate's row & col is >= 0; unsigned int is always >= 0, and any negative number (which here may only be -1) will overflow and become a very large number
                                                                                                                                                                                                 // calculate the first position only when the second position is valid (avoid unnecessary calculation)
                                                                                                                                                                                                 // if and only if the next two coordinates in that direction are both 1 (wall), can this direction be chosen for generation

      maze[first_row][first_col] = 0, maze[second_row][second_col] = 0;                      // change the two positions to path
      pathes[pathes_num][0] = first_row, pathes[pathes_num][1] = first_col, ++pathes_num;    // record the two positions
      pathes[pathes_num][0] = second_row, pathes[pathes_num][1] = second_col, ++pathes_num;  // -
      generate(maze, second_row, second_col, dir);                                           // continue to generate
    }
    if (pathes_num >= MAX_PATHES_NUM) return;        // stop when finished generating (avoid unnecessary calculation)
    dir = (dir + 1) % 4;                             // try next direction // FIXME make it real random
    if ((dir ^ 2) == from_dir) dir = (dir + 1) % 4;  // fix when choosing the opposite direction
  }
}

int main() {
  srand((unsigned)time(NULL));
  bool **maze = malloc(N * sizeof(bool *));
  for (unsigned row = 0; row < N; row++) maze[row] = memset(malloc(N * sizeof(bool)), 1, N);

  // problem generation starts here
  generate(maze, rand_between(0, N - 2) | 1, rand_between(0, N - 2) | 1, rand_between(0, 3));
  printf("%u\n", N);
  for (unsigned row = 0; row < N; row++) {
    for (unsigned col = 0; col < N; col++) printf("%c ", maze[row][col] ? '1' : '0');
    putchar('\n');
  }
  for (unsigned i = 0, rand = rand_between(0, MAX_PATHES_NUM - 1); i < 4; i++, rand = rand_between(0, MAX_PATHES_NUM - 1)) {
    printf("%u %u%c", pathes[rand][1], N - pathes[rand][0] - 1, i & 1 ? '\n' : ' ');
  }
  // problem generation ends here

  for (unsigned row = 0; row < N; row++) free(maze[row]);
  free(maze);
}