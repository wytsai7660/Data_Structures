#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RED "\e[31m"
#define GRN "\e[32m"
#define YEL "\e[33m"
#define BLU "\e[34m"
#define MAG "\e[35m"
#define CYN "\e[36m"

#define GRY "\e[90m"
#define ALT "\e[7;17m"  // highlight the current option
#define RESET "\e[0m"

int tmp_color(int n) {
  if (n == 0) return 90;
  return 31 + n % 6;
}

inline int max(int a, int b) { return a > b ? a : b; }

inline int min(int a, int b) { return a < b ? a : b; }

typedef struct link {
  int height;
  int a_row, a_col, b_row, b_col;
} link;

typedef struct request {
  int src, des;
} request;

void go(int r, int c, link **connection) {
  printf("(%d) %d-%d\n", connection[r][c].height, r, c);
  if (connection[r][c].height == 1) return;
  go(connection[r][c].a_row, connection[r][c].a_col, connection);
  go(connection[r][c].b_row, connection[r][c].b_col, connection);
}

int main() {
  int node_n, link_n, time, request_n, trash;
  scanf("%d%d%d%d", &node_n, &link_n, &time, &request_n);

  int *memory = malloc((unsigned)node_n * sizeof(int));
  for (int i = 0; i < node_n; i++) scanf("%d%d", &trash, &memory[i]);

  link **connection = malloc((unsigned)node_n * sizeof(link *));                            // TODO: , todo = node_n * (node_n - 1);
  for (int i = 0; i < node_n; i++) connection[i] = calloc((unsigned)node_n, sizeof(link));  // init to 0
  for (int i = 0, a, b; i < node_n; i++) {
    scanf("%d%d%d", &trash, &a, &b);
    connection[a][b].height = connection[b][a].height = 1;
  }

  for (int i = 0; i < node_n; i++) {
    for (int j = 0; j < node_n; j++) printf("\e[%dm%d " RESET, tmp_color(connection[i][j].height), connection[i][j].height);
    printf("\n");
  }

  bool change = true;  // maybe not needed
  while (change) {
    change = false;
    for (int i = 0; i < node_n; i++) {        // row_1
      for (int j = i + 1; j < node_n; j++) {  // row_2
        for (int k = 0; k < node_n; k++) {    // col
          if (connection[i][k].height && connection[k][j].height && max(connection[i][k].height, connection[k][j].height) + 1 < (connection[i][j].height == 0 ? INT_MAX : connection[i][j].height)) {
            connection[i][j] = connection[j][i] = (link){max(connection[i][k].height, connection[k][j].height) + 1, i, k, k, j}, change = true;
            printf("%d-%d -> %d-%d + %d-%d\n", i, j, i, k, k, j);
          }
        }
      }
    }
    printf("\n\n");
    for (int i = 0; i < node_n; i++) {
      for (int j = 0; j < node_n; j++) printf("\e[%dm%d " RESET, tmp_color(connection[i][j].height), connection[i][j].height);
      printf("\n");
    }
  }

  go(5, 7, connection);
  // printf("%d-%d + %d-%d", connection[5][7].a_row, connection[5][7].a_col, connection[5][7].b_row, connection[5][7].b_col);
  request **requests = malloc(request_n * sizeof(request *));
  for (int i = 0; i < request_n; i++) requests[i] = malloc(2 * sizeof(int));

  ////

  free(memory);
  for (int i = 0; i < node_n; i++) free(connection[i]);
  free(connection);
}