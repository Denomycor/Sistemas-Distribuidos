/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#ifndef _CLIENT_REDUNDANCY_H
#define _CLIENT_REDUNDANCY_H

#define ZDATALEN 200

#include <zookeeper/zookeeper.h>

/* Gets the server details from zookeeper and places them in the server param
 * Returns 0 if it executed without problems
 * Returns <0 if there was a problem
 */
int start_watcher();

#endif