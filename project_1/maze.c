#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned n = 11;  // row & col

int pathes[(n * n - n - n - 1) / 2][2];
int pathes_num = 0;

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
  unsigned dir = rand_between(from_dir + 3, from_dir + 5) % 4;  // choose a new direction randomly from 0 to 3, but avoid choosing the opposite direction
  // bool grow = 0;                                                // try at most three directions

  int new_row, new_col;                                                                      // include sign bit for negative number (out of the maze)
  for (int i = 0; i < 3; i++) {                                                              // try at most three directions
    new_row = (int)row + 2 * direction[dir][0], new_col = (int)col + 2 * direction[dir][1];  // next position
    if (new_row >= 0 && new_row < (int)n &&                                                  //
        new_col >= 0 && new_col < (int)n &&                                                  //
        maze[col + direction[dir][1]][row + direction[dir][0]] &&                            //
        maze[col + 2 * direction[dir][1]][row + 2 * direction[dir][0]]) {
      // the direction's next two coordinates are both 1 (wall), choose this direction as the new direction

      maze[row + direction[dir][0]][col + direction[dir][1]] = 0, maze[new_row][new_col] = 0;  // change the two positions to path

      pathes[pathes_num][0] = row + direction[dir][0], pathes[pathes_num][1] = col + direction[dir][1];
      pathes_num++;
      generate(maze, new_row, new_col, dir);
      // row += 2 * direction[dir][0], col += 2 * direction[dir][1], pos.third = dir, grow = 1;
      // IntTripleStack_push(pos_st, pos);  // push the current position into the position stack
      // break;                             // stop attempting other directions
    }

    // FIXME make it real random
    dir = (dir + 1) % 4;                             // try next direction
    if ((dir ^ 2) == from_dir) dir = (dir + 1) % 4;  // fix when choosing the opposite direction
  }
}

int main() {
  bool **maze = malloc(n * sizeof(int *));
  for (unsigned row = 0; row < n; row++) maze[row] = memset(malloc(n * sizeof(int)), 1, n);

  generate(maze, rand_between(0, n - 2) | 1, rand_between(0, n - 2) | 1, rand_between(0, 3));
  printf("%d") for (int i)
}