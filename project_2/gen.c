#include <stdio.h>
#include <stdlib.h>

int rand_between(int l, int r) { return rand() % (r - l + 1) + l; }  // get a random number between [l, r] 

void swap(void *a, void *b, size_t size) {
  if (a == b) return;
  char *ptr1 = a, *ptr2 = b;
  while (size--) {
    (*ptr1) ^= *ptr2, (*ptr2) ^= *ptr1, (*ptr1) ^= *ptr2;
    ptr1++, ptr2++;
  }
}

void generate(int router[], int size) {
  int *event = malloc(sizeof(int) * (size - 2));
  for (int i = 0; i < size - 2; i++) {
    event[i] = i + 1;
  }
  for (int i = 0; i < size - 2; i++) swap(event + i, event + rand_between(i, size - 3), sizeof(int));

  int ptr = 0;
  for (int i = 0; i < size - 2; i++) {
    router[ptr] = event[i];
    ptr = event[i];
  }
  router[ptr] = size - 1;
  router[size - 1] = -1;
  free(event);
}

int main(int argc, char *argv[]) {
  // if (argc != 3) {
  //   printf("Usage: %s <size> <seed>\nsize: num nodes to create\nseed: seed for random\n", argv[0]);
  //   return 1;
  // }

  int size = atoi(argv[1]);
  // int seed = atoi(argv[2]);
  unsigned seed = time(NULL);

  // int size = 20;
  srand(seed);
  int *network = malloc(sizeof(int) * size), *result = malloc(sizeof(int) * size);

  // generate(network, size, empty_rate);
  for (int i = 0; i < size - 1; i++) network[i] = i + 1;
  network[size - 1] = -1;
  generate(result, size);

  printf("%d\n", size);
  for (int i = 0; i < size; i++) {
    printf("%d ", network[i]);
  }
  printf("\n");
  for (int i = 0; i < size; i++) {
    printf("%d ", result[i]);
  }
  printf("\n");

  free(network);
  free(result);
}