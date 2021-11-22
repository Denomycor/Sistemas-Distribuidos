/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#ifndef _STATS_H
#define _STATS_H

#include "sys/time.h"
#include "statistics/stats-private.h"

//Start timer on t
void start_timing(struct timeval* t);

//Finish timer and return time in ms
double stop_timing(const struct timeval* t);

//Update the stats
void update_stats(struct statistics* stats, int op_code, double ms);


#endif