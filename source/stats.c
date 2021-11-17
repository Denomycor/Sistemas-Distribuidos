
#include "stats.h"

//Start timer on t
void start_timing(clock_t* t){
    *t = clock();
}

//Finish timer and leve on t the span between start_timing and stop_timing
void stop_timing(clock_t* t){
    *t = ((double) (clock() - *t)) / CLOCKS_PER_SEC;
}

//update the stats
void update_stats(stats_t* stats, int op_code, clock_t time){
    stats->total++;
    stats->avg += (time - stats->avg)/stats->total;
    stats->counter[(op_code/10)-1]++;
}