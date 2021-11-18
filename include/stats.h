#ifndef STATS_H
#define STATS_H

#include <time.h>

typedef struct{
    clock_t avg;
    size_t total;
    size_t counter[6];
}stats_t;

//Start timer on t
void start_timing(clock_t* t);

//Finish timer and leve on t the span between start_timing and stop_timing
void stop_timing(clock_t* t);

//Update the stats
void update_stats(stats_t* stats, int op_code, clock_t time);

//Return the avg time in seconds
double get_avg_time(const stats_t* stats);

#endif