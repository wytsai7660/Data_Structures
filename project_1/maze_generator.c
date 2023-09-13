

void gen_maze(int *map, int n, int x, int y, int from_dir) {}

// IntPair gen_maze(int *map, int n) {  // generate the maze
//                                      //   CharArray *events = events_shuffle(map->path);

//   // FIXME fix initial direction wrongly decided when map size is small
//   IntTriple pos = make_IntTriple(rand_between(0, n - 2) | 1, rand_between(0, n - 2) | 1, rand_between(0, 3));  // gen_center (x, y, from_dir)
//                                                                                                                //   IntTripleStack *pos_st = new_IntTripleStack();                                                                             // from_dir is saved as well
//                                                                                                                //   IntTripleStack_push(pos_st, pos);
//                                                                                                                //   IntPair player = make_IntPair(pos.second, pos.first);
//   map[pos.second][pos.first] = 'P';

//   int dir;
//   bool grow;

//   //   while (pos_st->size) {
//   //     pos = IntTripleStack_top(pos_st);
//   //     dir = rand_between(pos.third + 3, pos.third + 5) % 4, grow = 0;  // choose a new direction randomly from 0 to 3, but avoid choosing the opposite direction
//   //     for (int i = 0; i < 3; i++) {                                    // try at most three directions
//   //       if (pos.first + 2 * direction[dir][0] >= 0 && pos.first + 2 * direction[dir][0] < (int)n && pos.second + 2 * direction[dir][1] >= 0 && pos.second + 2 * direction[dir][1] < (int)n && map[pos.second + direction[dir][1]][pos.first + direction[dir][0]] == '@' && map[pos.second + 2 * direction[dir][1]][pos.first + 2 * direction[dir][0]] == '@') {
//   //         // the direction's next two coordinates are both @ (wall), choose this direction as the new direction
//   //         // change the two positions to path
//   //         map[pos.second + direction[dir][1]][pos.first + direction[dir][0]] = CharArray_pop_front(events), map[pos.second + 2 * direction[dir][1]][pos.first + 2 * direction[dir][0]] = CharArray_pop_front(events);
//   //         pos.first += 2 * direction[dir][0], pos.second += 2 * direction[dir][1], pos.third = dir, grow = 1;
//   //         IntTripleStack_push(pos_st, pos);  // push the current position into the position stack
//   //         break;                             // stop attempting other directions
//   //       }

//   //       // FIXME make it real random
//   //       dir = (dir + 1) % 4;                              // try next direction
//   //       if ((dir ^ 2) == pos.third) dir = (dir + 1) % 4;  // fix when choosing the opposite direction
//   //     }
//   //     if (!grow) IntTripleStack_pop(pos_st);  // can't go into any direction (dead end), pop out the top-most element
//   //   }

//   CharArray_clear(events);
//   IntTripleStack_clear(pos_st);
//   return player;
// }