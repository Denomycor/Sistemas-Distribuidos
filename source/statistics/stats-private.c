/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#include "statistics/stats-private.h"
#include <stdio.h>

//Prints the stats to standard output
void print_stats(const struct statistics* stats){
    printf("Average processing time: %fms\n", stats->avg);
    printf("Number of size ops: %u\n", stats->counter[0]);
    printf("Number of del ops: %u\n", stats->counter[1]);
    printf("Number of get ops: %u\n", stats->counter[2]);
    printf("Number of put ops: %u\n", stats->counter[3]);
    printf("Number of getkeys ops: %u\n", stats->counter[4]);
    printf("Number of table_print ops: %u\n", stats->counter[5]);
}