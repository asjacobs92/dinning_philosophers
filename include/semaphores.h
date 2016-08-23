#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define STATE_EATING 'E'
#define STATE_HUNGRY 'H'
#define STATE_THINKING 'T'

int is_even(int number);

void *philosopher(void *i);
void acquire_forks(int philosopher_pos);
void release_forks(int philosopher_pos);

#endif
