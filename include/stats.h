#ifndef STATS_H
#define STATS_H

#include <time.h>

typedef struct{
    union {
        clock_t avg;
        double sed;
    };
    size_t total;
    size_t counter[7];
}stats_t;

//Start timer on t
void start_timing(clock_t* t);

//Finish timer and leve on t the span between start_timing and stop_timing
void stop_timing(clock_t* t);

//update the stats
void update_stats(stats_t* stats, int op_code, clock_t time);


#endif