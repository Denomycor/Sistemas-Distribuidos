/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#ifndef _STATS_PRIVATE_H
#define _STATS_PRIVATE_H

struct statistics {
    double avg;
    unsigned int total;
    unsigned int counter[6];
};

//Prints the stats to standard output
void print_stats(const struct statistics* stats);

#endif