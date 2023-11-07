#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODE 2000

////////////////////////////////////////////////// stack //////////////////////////////////////////////////

// typedef struct {
//   int data[MAX_NODE + 5];
//   int top_index;  // INFO: -1 for empty
// } SimpleStack;    // just a very simple and basic stack

// inline void SimpleStack_push(SimpleStack *stack, int value) { stack->data[++stack->top_index] = value; }  // NOTE: no full check, use with caution

// inline int SimpleStack_pop(SimpleStack *stack) { return stack->data[(stack->top_index)--]; }  // NOTE: no empty check, use with caution

// inline bool SimpleStack_is_empty(SimpleStack *stack) { return stack->top_index == -1; }

////////////////////////////////////////////////// stack //////////////////////////////////////////////////

typedef struct {
  int begin, end;
} Segment;

int n, merged_n;
int *new, *new_dest, *reorder_table, *reverse_reorder_table;
bool *updated;

bool reorder(int *const original, const int *const original_dest) {
  reorder_table = memset(malloc(sizeof(int) * (unsigned)n), -1, sizeof(int) * (unsigned)n);
  reverse_reorder_table = malloc(sizeof(int) * (unsigned)n);  // but its size is merged_n
  new = malloc(sizeof(int) * (unsigned)n);                    // but its size is merged_n

  // build new, reorder_table, and reverse_reorder_table
  // update merged_n
  merged_n = 0;
  for (int i = 0; i != n - 1; i = original[i])  // traverse the original path
    if (original_dest[i] != -1) new[merged_n] = merged_n + 1, reorder_table[i] = merged_n, reverse_reorder_table[merged_n++] = i;
  new[merged_n] = -1, reorder_table[n - 1] = merged_n, reverse_reorder_table[merged_n++] = n - 1;  // dest node

  new_dest = malloc(sizeof(int) * (unsigned)merged_n);

  bool red = false;
  {  // build new_dest
     // update red
    int index = 0;
    for (int i = original_dest[0]; i != n - 1; i = original_dest[i])  // traverse the final path
      if (original[i] != -1) new_dest[index] = reorder_table[i], index = reorder_table[i];
      else original[i] = original_dest[i], red = true;
    new_dest[index] = reorder_table[n - 1], new_dest[merged_n - 1] = -1;  // dest node
  }

  // printf("reordered:\n");
  // for (int i = 0; i < merged_n; i++) printf("%d ", new[i]);
  // printf("\n");
  // printf("reorder_table:\n");
  // for (int i = 0; i < n; i++) printf("%d ", reorder_table[i]);
  // printf("\n");
  // printf("reverse_reorder_table:\n");
  // for (int i = 0; i < merged_n; i++) printf("%d ", reverse_reorder_table[i]);
  // printf("\n");
  // printf("new_dest:\n");
  // for (int i = 0; i < merged_n; i++) printf("%d ", new_dest[i]);
  // printf("\n");
  return red;
}

int find_longest_segments(Segment *segments, Segment *result, int seg_count) {
  int *DP = calloc((unsigned)seg_count, sizeof(int));

  int *from = memset(malloc(sizeof(int) * (unsigned)seg_count), -1, sizeof(int) * (unsigned)seg_count);

  // printf("from: \n");
  // for (int i = 0; i < merged_n; i++) printf("%d ", from[i]);

  int max_length = 0, end_index = -1;

  for (int i = 0; i < seg_count; i++) {
    for (int j = 0; j < i; j++)
      if (segments[i].begin >= segments[j].end && DP[i] < DP[j]) DP[i] = DP[j], from[i] = j;
    DP[i] += segments[i].end - segments[i].begin;
    if (DP[i] > max_length) max_length = DP[i], end_index = i;
  }

  // printf("\nfrom: \n");
  // for (int i = 0; i < seg_count; i++) printf("%d ", from[i]);
  // printf("\nDP: \n");
  // for (int i = 0; i < seg_count; i++) printf("%d ", DP[i]);
  // printf("Hi\n");

  int count = 0;
  while (end_index >= 0) {
    result[count++] = segments[end_index];
    // printf("result[%d]: %d %d\n", count - 1, segments[end_index].begin, segments[end_index].end);
    end_index = from[end_index];
  }

  // printf("3\n");

  free(DP);
  free(from);
  return count;
}

void shortcut_phase(bool *removed) {
  Segment *segments = malloc(sizeof(Segment) * (unsigned)merged_n);
  int count = 0;

  // printf("new_dest:\n");
  // for (int i = 0; i < merged_n; i++) printf("%d ", new_dest[i]);
  // printf("\n");
  // printf("updated:\n");
  // for (int i = 0; i < merged_n; i++) printf("%d ", updated[i] ? 1 : 0);
  // printf("\n");

  for (int i = 0; i < merged_n; i++) {
    // printf("i: %d\n", i);
    // for (int j = 0; j < merged_n; j++) printf("%d ", updated[j] ? 1 : 0);
    // printf("\n");
    if (new_dest[i] > i && !updated[i]) segments[count].begin = i, segments[count++].end = new_dest[i];
  }

  // printf("count: %d\n", count);
  // for (int i = 0; i < count; i++) printf("start: %d end: %d\n", segments[i].begin, segments[i].end);

  Segment *result = malloc(sizeof(Segment) * (unsigned)count);
  count = find_longest_segments(segments, result, count);
  // printf("count: %d\n", count);
  // for (int i = 0; i < count; i++) {
  //   printf("start: %d end: %d\n", result[i].begin, result[i].end);
  // }
  // printf("DP OK!\n");

  for (int i = 0; i < count; i++) {
    new[result[i].begin] = result[i].end;
    updated[result[i].begin] = true;
    for (int j = result[i].begin + 1; j < result[i].end; j++) removed[j] = true;
  }

  free(segments);
  free(result);

  // printf("update new OK!\n");
}

void prune_phase(bool *removed) {
  // for (int ptr = 0, flag = -1; ptr != merged_n - 1; ptr = new_dest[ptr]) {
  //   // printf("working on %d\n", ptr);
  //   if (removed[new_dest[ptr]] && flag == -1) {
  //     flag = ptr;

  //   } else if (flag != -1) {
  //     // printf("flag: %d, ptr: %d\n", flag, ptr);
  //     new_dest[flag] = new_dest[ptr];
  //     flag = -1;
  //   }
  // }
  int ptr = 0, flag = -1;
  while (ptr != merged_n - 1) {
    if (removed[new_dest[ptr]]) {
      if (flag == -1) {
        flag = ptr;
      }
    } else {
      if (flag != -1) {
        new_dest[flag] = new_dest[ptr];
        flag = -1;
      }
    }
    ptr = new_dest[ptr];
  }
}

int step = 0, ans[500][MAX_NODE + 5];

void store_ans(int *arr) {
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
  scanf("%d", &n);
  int *original = malloc(sizeof(int) * (unsigned)n);
  int *original_dest = malloc(sizeof(int) * (unsigned)n);
  for (int i = 0; i < n; i++) scanf("%d", original + i);
  for (int i = 0; i < n; i++) scanf("%d", original_dest + i);

  store_ans(original);  // 0

  if (reorder(original, original_dest)) store_ans(original);

  updated = calloc((unsigned)n, sizeof(bool) * (unsigned)merged_n);

  {                            // 1: handle all new nodes at once
    bool need_update = false;  // may not have any new nodes
    for (int i = 0; i < n; i++)
      if (original[i] == -1 && original_dest[i] != -1) original[i] = original_dest[i];
    if (need_update) store_ans(original);
  }

  bool *removed = malloc(sizeof(bool) * (unsigned)merged_n);
  for (int round = 1, updated_count = 0; updated_count < merged_n - 1; round++) {  // TODO

    if (round & 1) {
      memset(removed, 0, sizeof(bool) * (unsigned)merged_n);
      // printf("1 OK!\n");
      shortcut_phase(removed);
      // printf("2 OK!\n");
      for (int i = 0; i < merged_n; i++)
        if (updated[i]) {
          // printf("i = %d\n", i);
          original[reverse_reorder_table[i]] = original_dest[reverse_reorder_table[i]];
        }

      store_ans(original);

      // print_ans();
      // printf("shortcut OK!\n");

    } else {
      // printf("3 OK!\n");
      prune_phase(removed);
      // printf("4 OK!\n");

      for (int i = 0; i < merged_n; i++) updated[i] |= removed[i];

      for (int i = 0; i < merged_n; i++)
        if (updated[i]) original[reverse_reorder_table[i]] = original_dest[reverse_reorder_table[i]];
      store_ans(original);
      // pruned(result, removed);
      // bitset_union(updated, updated, removed);
      // bitset_copy(updates[round - 1], updated);
      // printf("round %d pruned, updated: %d\n", round, bitset_count(updated));
      // round++;
      // printf("prune OK!\n");
    }
    updated_count = 0;
    for (int i = 0; i < merged_n; i++)
      if (updated[i]) updated_count++;
  }

  {
    bool black = false;
    for (int i = 0; i < n; i++) {
      if (original[i] != original_dest[i]) {
        black = true;
        break;
      }
    }
    if (black) store_ans(original_dest);
  }
  print_ans();

  free(original);
  free(original_dest);
  free(new);
  free(new_dest);
  free(reorder_table);
  free(reverse_reorder_table);

  free(removed);
  free(updated);
}