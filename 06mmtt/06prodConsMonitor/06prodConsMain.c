#include "06prodConsMonitor.h"
#include <stdio.h>

#define N_PRODUCERS 2
#define MAX_PRODUCTS 6

void* producersFunction(void*);
void* consumersFunction(void*);

int main()
{
  pthread_t idProductores[2],idConsumidor;
  char str[30];
  int i;
  FIFO fifo;

  initFifo(&fifo);
  for(i=0;i<N_PRODUCERS;i++) pthread_create(&idProductores[i],NULL,producersFunction,(void*)&fifo);
  pthread_create(&idConsumidor,NULL,consumersFunction,(void*)&fifo);
  for(i=0;i<N_PRODUCERS;i++) pthread_join(idProductores[i],NULL);
  pthread_join(idConsumidor,NULL);
  destroyFifo(&fifo);
  printf("type \"Return\" to finish\n");
  fgets(str,sizeof(str),stdin); 
  return 0;
}

void* producersFunction(void* arg)
{
  int i;
  float f;
  PFIFO pFifo=(PFIFO)arg;

  for(i=0;i<MAX_PRODUCTS;i++)
  {
    put(pFifo,f);
    f+=15;    
  }
  return NULL;
}

void* consumersFunction(void* arg)
{
  int i;
  float f;
  PFIFO pFifo=(PFIFO)arg;

  for(i=0;i<MAX_PRODUCTS*N_PRODUCERS;i++)
  {
    get(pFifo,&f);
    printf("Consuming %f\n",f);
  }
  return NULL;
}
