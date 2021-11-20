#ifndef _STATS_H
#define _STATS_H

#include <time.h>
#include "statistics/stats-private.h"

//Start timer on t
void start_timing(clock_t* t);

//Finish timer and leve on t the span between start_timing and stop_timing
void stop_timing(clock_t* t);

//Update the stats
void update_stats(struct statistics* stats, int op_code, clock_t time);


#endif