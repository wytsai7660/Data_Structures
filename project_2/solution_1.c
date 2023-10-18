#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODE 10000

#define RED "\e[31m"
#define GRN "\e[32m"
#define YEL "\e[33m"
#define BLU "\e[34m"
#define MAG "\e[35m"
#define CYN "\e[36m"
#define WHT "\e[37m"
#define GRY "\e[90m"
#define RESET "\e[0m"

int last_print[MAX_NODE + 5];
void print_status(int status[], int n) {
  int space = (int)log10((double)n - 1) + 1;
  if (space < 2) space = 2;
  // printf("space: %d\n", space);

  for (int i = 0; i < space; i++) putchar(' ');
  printf(GRY "S |");
  for (int i = 1; i < n - 1; i++) printf(" %*d |", space, i);
  for (int i = 0; i < space; i++) putchar(' ');
  printf("D |\n" RESET);

  for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", status[i] == last_print[i] ? RESET : GRN, space, status[i]);
  printf(RESET "\n\n");
}

int main() {
  int n;
  scanf("%d", &n);
  int status[MAX_NODE + 5], dest[MAX_NODE + 5];
  for (int i = 0; i < n; i++) scanf("%d", &status[i]);
  memcpy(last_print, status, sizeof(last_print));
  for (int i = 0; i < n; i++) scanf("%d", &dest[i]);

  print_status(status, n);

  for (int i = 0; i < n; i++)
    if (status[i] == -1 && dest[i] != -1) status[i] = dest[i];

  print_status(status, n);

  // memcpy(status, dest, sizeof(last_print));

  // print_status(status, n);
}