#include "07prodConsMonitor.h"
#include <stdio.h>

void initFifo(PFIFO pFifo)
{
  pFifo->head=pFifo->tail=0;
  pFifo->numElems=0;
  pFifo->goOut=0;
  pthread_mutex_init(&pFifo->mtx,NULL);
  pthread_cond_init(&pFifo->condNotFull,NULL);
  pthread_cond_init(&pFifo->condNotEmpty,NULL);
}


int put(PFIFO pFifo, float elem)
{
  int mustLeave;

  pthread_mutex_lock(&pFifo->mtx);
  while(!pFifo->goOut && (pFifo->numElems == MAX_BUFFER_SIZE)) pthread_cond_wait(&pFifo->condNotFull,&pFifo->mtx);
  if(!pFifo->goOut)
  {
    printf("\tputing %f. Head= %d, tail= %d, nElems=%d\n",elem,pFifo->head, pFifo->tail, pFifo->numElems);
    pFifo->buffer[pFifo->head] = elem;
    pFifo->head = (pFifo->head+1) % MAX_BUFFER_SIZE;
    pFifo->numElems++;
    if(pFifo->numElems == 1) pthread_cond_signal(&pFifo->condNotEmpty);
  }
  mustLeave=pFifo->goOut;
  pthread_mutex_unlock(&pFifo->mtx);
  return mustLeave;
}

int get(PFIFO pFifo, float* pElem)
{
  int mustLeave=0;

  pthread_mutex_lock(&pFifo->mtx);
  while(!pFifo->goOut && (pFifo->numElems == 0)) pthread_cond_wait(&pFifo->condNotEmpty,&pFifo->mtx);
  if(pFifo->numElems != 0)
  {
    *pElem = pFifo->buffer[pFifo->tail];
    printf("\tgetting %f. Head= %d, tail= %d, nElems=%d\n",*pElem,pFifo->head, pFifo->tail, pFifo->numElems);
    pFifo->tail = (pFifo->tail+1) % MAX_BUFFER_SIZE;
    pFifo->numElems--;
    if(pFifo->numElems == (MAX_BUFFER_SIZE -1)) pthread_cond_signal(&pFifo->condNotFull);
  }
  else mustLeave=pFifo->goOut;
  pthread_mutex_unlock(&pFifo->mtx);
  return mustLeave;
}


void interrupt(PFIFO pFifo)
{
  pthread_mutex_lock(&pFifo->mtx);
  pFifo->goOut=1;
  pthread_cond_broadcast(&pFifo->condNotFull);
  pthread_cond_broadcast(&pFifo->condNotEmpty);
  pthread_mutex_unlock(&pFifo->mtx);
}

void destroyFifo(PFIFO pFifo)
{
  pthread_cond_destroy(&pFifo->condNotFull);
  pthread_cond_destroy(&pFifo->condNotEmpty);
  pthread_mutex_destroy(&pFifo->mtx);
}


