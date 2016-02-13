#include "07prodConsMonitor.h"
#include <stdio.h>
#include <unistd.h>

#define N_PRODUCERS 6
#define N_CONSUMERS 6

void* producersFunction(void*);
void* consumersFunction(void*);

int main()
{
  pthread_t idProducer[N_PRODUCERS],idConsumer[N_CONSUMERS];
  char str[30];
  int i;
  FIFO fifo;

  initFifo(&fifo);
  for(i=0;i<N_PRODUCERS;i++) pthread_create(&idProducer[i],NULL,producersFunction,(void*)&fifo);
  for(i=0;i<N_PRODUCERS;i++) pthread_create(&idConsumer[i],NULL,consumersFunction,(void*)&fifo);
  printf("type \"Return\" to start finishing\n");
  fgets(str,sizeof(str),stdin);
  interrupt(&fifo);
  for(i=0;i<N_PRODUCERS;i++) pthread_join(idProducer[i],NULL);
  for(i=0;i<N_PRODUCERS;i++) pthread_join(idConsumer[i],NULL);
  destroyFifo(&fifo);
  printf("end of main\n");
  return 0;
}

void* producersFunction(void* arg)
{
  float f;
  PFIFO pFifo=(PFIFO)arg;

  f=0;
  while(!put(pFifo,f))
  {
    f+=15;    
  }
  return NULL;
}

void* consumersFunction(void* arg)
{
  float f;
  PFIFO pFifo=(PFIFO)arg;

  while(!get(pFifo,&f))
  {
    printf("Consuming %f\n",f);
    sleep(1);
  }
  return NULL;
}
