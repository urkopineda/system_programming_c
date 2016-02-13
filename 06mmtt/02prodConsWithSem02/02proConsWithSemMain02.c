#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#include "fifo.h"

#define N_PRODUCTS  10
#define N_PRODUCERS 2

void* producersFunction(void*);
void* consumersFunction(void*);

int main()
{
  pthread_t idProducers[N_PRODUCERS];
  pthread_t idConsumer;
  char str[128];
  int i;
  FIFO fifo;

  initFifo(&fifo);
  for(i=0;i<N_PRODUCERS;i++) pthread_create(&idProducers[i],NULL,producersFunction,(void*)&fifo);
  pthread_create(&idConsumer,NULL,consumersFunction,(void*)&fifo);
  for(i=0;i<N_PRODUCERS;i++) pthread_join(idProducers[i],NULL);
  pthread_join(idConsumer,NULL);
  cleanFifo(&fifo);
  printf("type \"Return\" to finishn");
  fgets(str,sizeof(str),stdin);
  return 0;
}

void* producersFunction(void* arg)
{
  float f=0;
  int i;
  PFIFO pQueue=(PFIFO)arg;

  for(i=0;i<N_PRODUCTS;i++)
  {
    //sleep(1);
    put(pQueue,f);
    f+=15;
  }
  return NULL;
}

void* consumersFunction(void* arg)
{
  float f;
  int i;
  PFIFO pQueue=(PFIFO)arg;

  for(i=0;i<N_PRODUCERS*N_PRODUCTS;i++)
  {
    sleep(1);
    get(pQueue,&f);
    printf("Consuming %f\n",f);
  }
  return NULL;
}

