/* Grupo 16
 * Afonso Esteves 54394
 * Vicente Sousa 55386
 * João Anjos 54476
 */

#ifndef _CLIENT_REDUNDANCY_H
#define _CLIENT_REDUNDANCY_H

#define ZDATALEN 200

#include <zookeeper/zookeeper.h>

/* Returns the state of the connection
 * Return 1 connected
 * Return 0 not connected
 */
int is_connected();

/* Starts the redundancy managers
 * Return 0 ok
 * Return <0 in case of error
 */
int init_redundancy_manager();

/* Gets the server details from zookeeper and places them in the server param
 * Returns 0 if it executed without problems
 * Returns <0 if there was a problem
 */
int start_watcher();

/* Returns 1 if the function has never been called after an update to the watched value 
 * Return 0 otherwise
 */
int is_new();

/* Stops the redundancy managers
 * Return 0 ok
 * Return <0 in case of error
 */
void close_redundancy_manager();

#endif