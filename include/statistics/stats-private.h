#ifndef _STATS_PRIVATE_H
#define _STATS_PRIVATE_H

#include <time.h>

struct statistics {
    clock_t avg;
    size_t total;
    size_t counter[6];
};

//Return the avg time in seconds
double get_avg_time(const struct statistics* stats);

//Print the counter to standard output
void print_counter(const struct statistics* stats);

#endif