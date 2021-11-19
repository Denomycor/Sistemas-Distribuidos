
#ifndef _STATS_PRIVATE_H
#define _STATS_PRIVATE_H

#include "statistics/stats.h"
#include <time.h>

struct statistics {
    clock_t avg;
    size_t total;
    size_t counter[6];
};

#endif