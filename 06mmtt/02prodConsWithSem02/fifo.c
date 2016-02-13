#include "fifo.h"


void initFifo(PFIFO pFifo)
{
  pFifo->head=pFifo->tail=pFifo->numElems=0;
  sem_init(&pFifo->sMe, 0, 1);
  sem_init(&pFifo->sP, 0, MAX_BUFFER_SIZE);
  sem_init(&pFifo->sC, 0, 0);
}

void cleanFifo(PFIFO pFifo)
{
  sem_destroy(&pFifo->sMe);
  sem_destroy(&pFifo->sP);
  sem_destroy(&pFifo->sC);
}

void put(PFIFO pFifo, float element)
{
  sem_wait(&pFifo->sP);
  sem_wait(&pFifo->sMe);
  pFifo->buffer[pFifo->head]=element;
  pFifo->head=(pFifo->head+1)%MAX_BUFFER_SIZE;
  pFifo->numElems++;
  sem_post(&pFifo->sMe);
  sem_post(&pFifo->sC);
}

void get(PFIFO pFifo, float *pElement)
{
  sem_wait(&pFifo->sC);
  sem_wait(&pFifo->sMe);
  *pElement=pFifo->buffer[pFifo->tail];
  pFifo->tail=(pFifo->tail+1)%MAX_BUFFER_SIZE;
  pFifo->numElems--;
  sem_post(&pFifo->sP);
  sem_post(&pFifo->sMe);
}
