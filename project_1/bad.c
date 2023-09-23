#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int arr[10] = {};
  int i = 0;
  arr[i] = i++; /* 未定义行为 */
}