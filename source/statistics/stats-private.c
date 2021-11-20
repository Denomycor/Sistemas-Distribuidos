#include "statistics/stats-private.h"
#include <stdio.h>

//Return the avg time in seconds
double get_avg_time(const struct statistics* stats){
    return ((double)stats->avg) / CLOCKS_PER_SEC;
}

//Print the counter to standard output
void print_counter(const struct statistics* stats){
    printf("Number of size ops: %lu\n", stats->counter[0]);
    printf("Number of del ops: %lu\n", stats->counter[1]);
    printf("Number of get ops: %lu\n", stats->counter[2]);
    printf("Number of put ops: %lu\n", stats->counter[3]);
    printf("Number of getkeys ops: %lu\n", stats->counter[4]);
    printf("Number of table_print ops: %lu\n", stats->counter[5]);
}