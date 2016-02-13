#include "06prodConsMonitor.h"
#include <stdio.h>


void initFifo(PFIFO pFifo)
{
  pFifo->head=pFifo->tail=0;
  pFifo->numElems=0;
  pthread_mutex_init(&pFifo->mtx,NULL);
  pthread_cond_init(&pFifo->condNotFull,NULL);
  pthread_cond_init(&pFifo->condNotEmpty,NULL);
}

int put(PFIFO pFifo, float elem)
{
  pthread_mutex_lock(&pFifo->mtx);
  while(pFifo->numElems == MAX_BUFFER_SIZE) pthread_cond_wait(&pFifo->condNotFull,&pFifo->mtx);
  printf("\tputing %f. Head= %d, tail= %d, nElems=%d\n",elem,pFifo->head, pFifo->tail, pFifo->numElems);
  pFifo->buffer[pFifo->head] = elem;
  pFifo->head = (pFifo->head+1) % MAX_BUFFER_SIZE;
  pFifo->numElems++;
  if(pFifo->numElems == 1) pthread_cond_signal(&pFifo->condNotEmpty);
  pthread_mutex_unlock(&pFifo->mtx);
  return 0;
}


int get(PFIFO pFifo, float* pElem)
{
  pthread_mutex_lock(&pFifo->mtx);
  while(pFifo->numElems == 0) pthread_cond_wait(&pFifo->condNotEmpty,&pFifo->mtx);
  *pElem = pFifo->buffer[pFifo->tail];
  printf("\tgetting %f. Head= %d, tail= %d, nElems=%d\n",*pElem,pFifo->head, pFifo->tail, pFifo->numElems);
  pFifo->tail = (pFifo->tail+1) % MAX_BUFFER_SIZE;
  pFifo->numElems--;
  //if(pFifo->numElems == (MAX_BUFFER -1)) pthread_cond_broadcast(&pFifo->condNotFull);
  if(pFifo->numElems == (MAX_BUFFER_SIZE -1)) pthread_cond_signal(&pFifo->condNotFull);
  pthread_mutex_unlock(&pFifo->mtx);
  return 0;
}

void destroyFifo(PFIFO pFifo)
{
  pthread_cond_destroy(&pFifo->condNotFull);
  pthread_cond_destroy(&pFifo->condNotEmpty);
  pthread_mutex_destroy(&pFifo->mtx);
}


