  int ptr = 0, flag = -1;
  while (ptr != -1) {
    if (is_in_bitset(removed, result[ptr])) {
      if (flag == -1) {
        flag = ptr;
      }
    } else {
      if (flag != -1) {
        result[flag] = result[ptr];
        flag = -1;
      }
    }
    ptr = result[ptr];
  }