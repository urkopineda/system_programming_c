#include "communicationMonitor.h"
#include <stdio.h>

void init(PCOMMUNICATION_MONITOR pMon, int initialIddleTasks)
{
  pMon->head=pMon->tail=0;
  pMon->numElems=0;
  pMon->goOut=0;
  pMon->nIddleTasks=initialIddleTasks;
  pthread_mutex_init(&pMon->mtx,NULL);
  pthread_cond_init(&pMon->condNotFull,NULL);
  pthread_cond_init(&pMon->condNotEmpty,NULL);
  pthread_cond_init(&pMon->condSomeIddleTask,NULL);
}


int put(PCOMMUNICATION_MONITOR pMon, int sock)
{
  int mustLeave;

  pthread_mutex_lock(&pMon->mtx);
  while(!pMon->goOut && (pMon->numElems == MAX_BUFFER_SIZE)) pthread_cond_wait(&pMon->condNotFull,&pMon->mtx);
  if(!pMon->goOut)
  {
    pMon->buffer[pMon->head] = sock;
    pMon->head = (pMon->head+1) % MAX_BUFFER_SIZE;
    pMon->numElems++;
    pthread_cond_signal(&pMon->condNotEmpty);
  }
  mustLeave=pMon->goOut;
  pthread_mutex_unlock(&pMon->mtx);
  return mustLeave;
}

int get(PCOMMUNICATION_MONITOR pMon, int *pSock)
{
  int mustLeave=0;

  pthread_mutex_lock(&pMon->mtx);
  while(!pMon->goOut && (pMon->numElems == 0)) pthread_cond_wait(&pMon->condNotEmpty,&pMon->mtx);
  if(pMon->numElems != 0)
  {
    *pSock = pMon->buffer[pMon->tail];
    pMon->tail = (pMon->tail+1) % MAX_BUFFER_SIZE;
    pMon->numElems--;
    pthread_cond_signal(&pMon->condNotFull);
  }
  else mustLeave=pMon->goOut;
  pthread_mutex_unlock(&pMon->mtx);
  return mustLeave;
}

int waitIddleTask(PCOMMUNICATION_MONITOR pMon)
{
  int mustLeave;

  pthread_mutex_lock(&pMon->mtx);
  while(!pMon->goOut && (pMon->nIddleTasks==0)) 
    pthread_cond_wait(&pMon->condSomeIddleTask,&pMon->mtx);
  pMon->nIddleTasks--;
  mustLeave=pMon->goOut;
  pthread_mutex_unlock(&pMon->mtx);
  return mustLeave;
}

void addIddleTask(PCOMMUNICATION_MONITOR pMon)
{  
  pthread_mutex_lock(&pMon->mtx);
  pMon->nIddleTasks++;
  pthread_cond_signal(&pMon->condSomeIddleTask);
  pthread_mutex_unlock(&pMon->mtx);
}


void interrupt(PCOMMUNICATION_MONITOR pMon)
{
  pthread_mutex_lock(&pMon->mtx);
  pMon->goOut=1;
  pthread_cond_broadcast(&pMon->condNotFull);
  pthread_cond_broadcast(&pMon->condNotEmpty);
  pthread_cond_broadcast(&pMon->condSomeIddleTask);
  pthread_mutex_unlock(&pMon->mtx);
}

void destroyFifo(PCOMMUNICATION_MONITOR pMon)
{
  pthread_cond_destroy(&pMon->condNotFull);
  pthread_cond_destroy(&pMon->condNotEmpty);
  pthread_cond_destroy(&pMon->condSomeIddleTask);
  pthread_mutex_destroy(&pMon->mtx);
}


