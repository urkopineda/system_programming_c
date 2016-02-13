#ifndef PRODCONSMONITOR07_H
#define PRODCONSMONITOR07_H

#include <pthread.h>
#include <stdio.h>
#include "commWrapper.h"

#define MAX_BUFFER_SIZE 30

typedef struct s_Fifo {
  int head;
  int tail;
  int numElems;
  PMESSAGE buffer[MAX_BUFFER_SIZE];
  pthread_mutex_t mtx;
  pthread_cond_t condNotFull;
  pthread_cond_t condNotEmpty;
  int goOut;
} FIFO,*PFIFO;

void initFifo(PFIFO pFifo);
int put(PFIFO pFifo, PMESSAGE pMsg);
int get(PFIFO pFifo, PMESSAGE pMsg);
void interrupt(PFIFO pFifo);
void destroyFifo(PFIFO pFifo);

#endif