#include <stdio.h>
#include <stdlib.h>

int main() {
  void *x = malloc(10);
  x = NULL;
  //   char *x = (char *)malloc(10 * sizeof(char *));
  //   free(x);
  //   return x[5];
  printf("%p\n", x);
}