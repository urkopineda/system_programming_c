#ifndef PRODCONSMONITOR06_H
#define PRODCONSMONITOR06_H

#include <pthread.h>

#define MAX_BUFFER_SIZE 5

typedef struct s_Fifo
{
  int head;
  int tail;
  int numElems;
  float buffer[MAX_BUFFER_SIZE];
  pthread_mutex_t mtx;
  pthread_cond_t condNotFull;
  pthread_cond_t condNotEmpty;
}FIFO,*PFIFO;

void initFifo(PFIFO pFifo);
int put(PFIFO pFifo, float elem);
int get(PFIFO pFifo, float * pElem);
void destroyFifo(PFIFO pFifo);

#endif
