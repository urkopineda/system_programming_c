#ifndef QUEUE_H
#define QUEUE_H

#include <semaphore.h>

#define MAX_BUFFER_SIZE 5

typedef struct s_Queue
{
  int head;
  int tail;
  int numElems;
  float buffer[MAX_BUFFER_SIZE];
  sem_t  sMe, sP, sC;
}FIFO,*PFIFO;

void initFifo(PFIFO pFifo);
void cleanFifo(PFIFO pFifo);
void get(PFIFO pFifo, float *pElement);
void put(PFIFO pFifo, float element);

#endif
