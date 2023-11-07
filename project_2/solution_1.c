#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODE 2000

int n;

#define DEBUG

////////////////////////////////////////////////// to be deleted //////////////////////////////////////////////////

#ifdef DEBUG
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
void print_status(int status[]) {
  int space = (int)log10((double)n - 1) + 1;
  if (space < 2) space = 2;
  // printf("space: %d\n", space);

  for (int i = 0; i < space; i++) putchar(' ');
  printf(GRY "S |");
  for (int i = 1; i < n - 1; i++) printf(" %*d |", space, i);
  for (int i = 0; i < space; i++) putchar(' ');
  printf("D |\n" RESET);

  for (int i = 0; i < n; i++) {
    printf(" %s%*d " GRY "|", status[i] == last_print[i] ? YEL : GRN, space, status[i]);
    last_print[i] = status[i];
  }
  printf(RESET "\n\n");
}
#else
void print_status() {
  printf("\e[31mremove the call of print_status()!\n");
  exit(1);
}
#endif

////////////////////////////////////////////////// to be deleted //////////////////////////////////////////////////

typedef struct SimpleStack {
  int data[MAX_NODE + 5];
  int top_index;  // INFO: -1 for empty
} SimpleStack;    // just a very simple and basic stack

inline void SimpleStack_push(SimpleStack *stack, int value) { stack->data[++stack->top_index] = value; }  // NOTE: no full check, use with caution

inline int SimpleStack_pop(SimpleStack *stack) { return stack->data[(stack->top_index)--]; }  // NOTE: no empty check, use with caution

inline bool SimpleStack_is_empty(SimpleStack *stack) { return stack->top_index == -1; }

int step = 0, ans[500][MAX_NODE + 5];

void store_ans(int arr[]) {
  for (int i = 0; i < n; i++) ans[step][i] = arr[i];
  step++;
}

void print_ans() {
  printf("%d\n", step);
  for (int i = 0; i < step; i++) {
    for (int j = 0; j < n; j++) printf("%d ", ans[i][j]);
    putchar('\n');
  }
}

int main() {
  int status[MAX_NODE + 5], dest[MAX_NODE + 5], merge_table[MAX_NODE + 5],  // NOTE: not initialized, should be assigned before use
      index_to_order[MAX_NODE + 5], order_to_index[MAX_NODE + 5];           // NOTE: I should definetly use a linked lists instead of arrays
  bool to_be_updated[MAX_NODE + 5] = {}, is_forward[MAX_NODE + 5] = {};     // TODO: try remove to_be_updated[] maybe?

  scanf("%d", &n);
  for (int i = 0; i < n; i++) scanf("%d", &status[i]);
  for (int i = 0; i < n; i++) scanf("%d", &dest[i]);

  memset(index_to_order, -1, sizeof(int) * (unsigned)n);
  memset(order_to_index, -1, sizeof(int) * (unsigned)n);

  {                              // merge_table (also handle index_to_order[] and to_be_updated[] at the same time)
                                 // NOTE: merge forward, i.e. merge_table[x] == x means x is the final node
    merge_table[n - 1] = n - 1;  // no need to handle the src and des, as they are always impossible to merge
    SimpleStack stack = {.top_index = -1};
    int count = 0;
    for (int i = 0; i < n - 1; i = status[i]) {  // traverse the original path
      SimpleStack_push(&stack, i);
      if (dest[i] != -1) {
        while (!SimpleStack_is_empty(&stack)) merge_table[SimpleStack_pop(&stack)] = i;
        index_to_order[i] = count, order_to_index[count++] = i;  // order
        if (status[i] != dest[i]) to_be_updated[i] = true;
      }
    }
    while (!SimpleStack_is_empty(&stack)) merge_table[SimpleStack_pop(&stack)] = n - 1;
    index_to_order[n - 1] = count;
    for (int i = dest[0]; i < n - 1; i = dest[i]) {  // traverse the final path
      SimpleStack_push(&stack, i);
      if (status[i] != -1)
        while (!SimpleStack_is_empty(&stack)) merge_table[SimpleStack_pop(&stack)] = i;
    }
    while (!SimpleStack_is_empty(&stack)) merge_table[SimpleStack_pop(&stack)] = n - 1;
  }

  for (int i = 0; i < n - 1; i++)  // is_forward
    if (to_be_updated[merge_table[i]] && index_to_order[merge_table[dest[i]]] > index_to_order[merge_table[i]]) is_forward[i] = true;

#ifdef DEBUG
  {
    int space = (int)log10((double)n - 1) + 1;
    if (space < 2) space = 2;

    for (int i = 0; i < space; i++) putchar(' ');
    printf(GRY "S |");
    for (int i = 1; i < n - 1; i++) printf(" %*d |", space, i);
    for (int i = 0; i < space; i++) putchar(' ');
    printf("D |\n" RESET "merge_table\n");

    for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", merge_table[i] != i ? YEL : RED, space, merge_table[i]);
    printf(RESET "\nindex_to_order\n");

    for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", index_to_order[i] != -1 ? YEL : RED, space, index_to_order[i]);
    printf(RESET "\norder_to_index\n");

    for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", order_to_index[i] != -1 ? YEL : RED, space, order_to_index[i]);
    printf(RESET "\nto_be_updated\n");

    for (int i = 0; i < n; i++) printf(" %s%*c " GRY "|", to_be_updated[i] ? YEL : RED, space, to_be_updated[i] ? 'T' : 'F');
    printf(RESET "\nis_forward\n");

    for (int i = 0; i < n; i++) printf(" %s%*c " GRY "|", is_forward[i] ? YEL : RED, space, is_forward[i] ? 'T' : 'F');
    // printf(RESET "\nDP\n");

    // for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", DP[i] ? YEL : RED, space, DP[i]);
    // printf(RESET "\nfrom\n");

    // for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", from[i] != -1 ? YEL : RED, space, from[i]);
    printf(RESET "\n\n");
  }

  memset(last_print, 0, sizeof(last_print));  // ok because 0
  printf("=========================\n\n");

  memcpy(last_print, status, sizeof(last_print));
  printf("initial status\n");
  print_status(status);
#endif

  store_ans(status);  // 0

  {                            // 1: handle all new nodes at once
    bool need_update = false;  // may not have any new nodes
    for (int i = 0; i < n; i++)
      if (status[i] == -1 && dest[i] != -1) status[i] = dest[i], need_update = true;
    if (need_update) store_ans(status);

#ifdef DEBUG
    if (need_update) {
      printf("1st round\n");
      print_status(status);
    }
#endif
  }

  int DP[MAX_NODE + 5], from[MAX_NODE + 5], status_tmp[MAX_NODE + 5];
  bool valid, need_update;
  // NOTE: I don't have enough time to come up with a more elegant way, which is why this part looks like shit. Fuck the deadline.🖕
  while (true) {
    memset(DP, 0, sizeof(int) * (unsigned)n);
    memset(from, -1, sizeof(int) * (unsigned)n);
    valid = false;

#ifdef DEBUG
    {
      int space = (int)log10((double)n - 1) + 1;
      if (space < 2) space = 2;

      for (int i = 0; i < space; i++) putchar(' ');
      printf(GRY "S |");
      for (int i = 1; i < n - 1; i++) printf(" %*d |", space, i);
      for (int i = 0; i < space; i++) putchar(' ');
      printf("D |\n" RESET "merge_table\n");

      for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", merge_table[i] != i ? YEL : RED, space, merge_table[i]);
      printf(RESET "\nindex_to_order\n");

      for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", index_to_order[i] != -1 ? YEL : RED, space, index_to_order[i]);
      printf(RESET "\norder_to_index\n");

      for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", order_to_index[i] != -1 ? YEL : RED, space, order_to_index[i]);
      printf(RESET "\nto_be_updated\n");

      for (int i = 0; i < n; i++) printf(" %s%*c " GRY "|", to_be_updated[i] ? YEL : RED, space, to_be_updated[i] ? 'T' : 'F');
      printf(RESET "\nis_forward\n");

      for (int i = 0; i < n; i++) printf(" %s%*c " GRY "|", is_forward[i] ? YEL : RED, space, is_forward[i] ? 'T' : 'F');
      // printf(RESET "\nDP\n");

      // for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", DP[i] ? YEL : RED, space, DP[i]);
      // printf(RESET "\nfrom\n");

      // for (int i = 0; i < n; i++) printf(" %s%*d " GRY "|", from[i] != -1 ? YEL : RED, space, from[i]);
      printf(RESET "\n\n");
    }
#endif
    ////////////////////////////////////////////////// DP //////////////////////////////////////////////////
    for (int i = 0, current_max = 0, tmp, counter = 0; i != n - 1 && counter < n + 5; i = order_to_index[index_to_order[i] + 1], counter++) {  // DP (from)
      if (!to_be_updated[i]) continue;
      // printf("Handle %d\n", i);
      if (DP[i]) current_max = DP[i];

      if (is_forward[i]) {
        // printf("is_forward[%d] = true\n", i);
        tmp = current_max + index_to_order[merge_table[dest[i]]] - index_to_order[merge_table[i]], valid = true;
        if (tmp > DP[merge_table[dest[i]]]) DP[merge_table[dest[i]]] = tmp, from[merge_table[dest[i]]] = i;
      }
      // print_status(DP);
      // for (int l = 0; l < 1e9; l++)
      //   ;
    }
    // printf("DP ok!\n");

#ifdef DEBUG
    // print_status(DP);
    // print_status(from);
#endif

    if (!valid) break;

    memcpy(status_tmp, status, sizeof(status_tmp));

    need_update = false;

    for (int i = n - 1; i != 0; i = from[i]) {  // TODO: maybe rewrite this part in a completly different way
      // printf("\n%d ", i);
      while (!DP[i] && i) {  // BUG only 2 segment at most
        i = order_to_index[index_to_order[i] - 1];
        // printf("-> %d ", i);
        // for (int l = 0; l < 1e9; l++)
        //   ;
      }
      if (i == 0) break;

      status_tmp[from[i]] = dest[from[i]];
      // printf("%d to prev[%d]\n", index_to_order[from[i]], index_to_order[i]);
      // is_forward[from[i]] = false;
      for (int j = index_to_order[from[i]]; j < index_to_order[i]; j++) {
        // printf("%d\n", order_to_index[j]);
        if (status[order_to_index[j]] != dest[order_to_index[j]]) {
          // printf(" %d %d\n", status[order_to_index[j]], dest[order_to_index[j]]);
          // printf("update %d\n", order_to_index[j]);
          status[order_to_index[j]] = dest[order_to_index[j]], need_update = j != index_to_order[from[i]], to_be_updated[order_to_index[j]] = false, is_forward[order_to_index[j]] = false;
        }
        // if (to_be_updated[order_to_index[j]]) to_be_updated[order_to_index[j]] = false;
      }
      is_forward[from[i]] = false;
    }

#ifdef DEBUG
    printf("2nd round\n");
    print_status(status_tmp);

    printf("3rd round\n");
    print_status(status);
#endif

    store_ans(status_tmp);
    if (need_update) store_ans(status);

    // for (int k = 0; k < 1e9; k++)
    // for (int l = 0; l < 1e2; l++)
    // ;  // delay
  }

  //   {                            // -2: handle the rest
  //     bool need_update = false;  // may not have any rest nodes
  //     for (int i = 0; i < n; i++)
  //       if (status[i] != -1 && dest[i] != -1 && status[i] != dest[i]) status[i] = dest[i], need_update = true;
  //     if (need_update) store_ans(status);

  // #ifdef DEBUG
  //     if (need_update) {
  //       printf("-2nd round\n");
  //       print_status(status);
  //     }
  // #endif
  //   }

  {                            // -1: handle the remaining nodes
    bool need_update = false;  // may not have any rest nodes
    for (int i = 0; i < n; i++)
      if (status[i] != dest[i]) need_update = true;  // can be done by simply use dest[]
    if (need_update) store_ans(dest);

#ifdef DEBUG
    if (need_update) {
      printf("-1st round\n");
      print_status(dest);
    }
#endif
  }

  print_ans();
}