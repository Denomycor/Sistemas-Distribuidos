#ifndef _STATS_H
#define _STATS_H

#include <time.h>

typedef struct{
    clock_t avg;
    size_t total;
    size_t counter[6];
}statistics;

//Start timer on t
void start_timing(clock_t* t);

//Finish timer and leve on t the span between start_timing and stop_timing
void stop_timing(clock_t* t);

//Update the stats
void update_stats(statistics* stats, int op_code, clock_t time);

//Return the avg time in seconds
double get_avg_time(const statistics* stats);

#endif