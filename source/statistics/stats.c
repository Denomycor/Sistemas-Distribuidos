#include "statistics/stats.h"

//Start timer on t
void start_timing(clock_t* t){
    *t = clock();
}

//Finish timer and leve on t the span between start_timing and stop_timing
void stop_timing(clock_t* t){
    *t = clock() - *t;
}

//Update the stats
void update_stats(statistics* stats, int op_code, clock_t time){
    stats->total++;
    stats->avg += (time - stats->avg)/stats->total;
    stats->counter[(op_code/10)-1]++;
}

//Return the avg time in seconds
double get_avg_time(const statistics* stats){
    return ((double)stats->avg) / CLOCKS_PER_SEC;
}