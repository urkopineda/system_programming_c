#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define MAX_BUFFER_SIZE 5
#define MAX_PRODUCTS  10
#define N_PRODUCERS 2

typedef struct s_Buffer
{
  int head;
  int tail;
  int numElems;
  float buffer[MAX_BUFFER_SIZE];
  sem_t  sMe, sP, sC;
}FIFO,*PFIFO;

void* producersFunction(void*);
void* ConsumersFunction(void*);
void initFIFO(PFIFO ptail);
void cleanFIFO(PFIFO ptail);

int main()
{
  pthread_t idProducer[N_PRODUCERS];
  pthread_t idConsumer;
  char str[128];
  int i;
  FIFO fifo;

  initFIFO(&fifo);
  for(i=0;i<N_PRODUCERS;i++) pthread_create(&idProducer[i],NULL,producersFunction,(void*)&fifo);
  pthread_create(&idConsumer,NULL,ConsumersFunction,(void*)&fifo);
  for(i=0;i<N_PRODUCERS;i++) pthread_join(idProducer[i],NULL);
  pthread_join(idConsumer,NULL);
  cleanFIFO(&fifo);
  printf("type \"Return\" to finish\n");
  fgets(str,sizeof(str),stdin); 
  return 0;
}

void* producersFunction(void* arg)
{
  float f=1;
  int i;
  PFIFO pFifo=(PFIFO)arg;

  for(i=0;i<MAX_PRODUCTS;i++)
  {
    //sleep(1);
    sem_wait(&pFifo->sP);
    sem_wait(&pFifo->sMe);
    pFifo->buffer[pFifo->head]=f;
    pFifo->head=(pFifo->head+1)%MAX_BUFFER_SIZE;
    pFifo->numElems++;
    sem_post(&pFifo->sMe);
    sem_post(&pFifo->sC);
    f+=15;
  }
  return NULL;
}

void* ConsumersFunction(void* arg)
{
  float f;
  int i;
  PFIFO pFifo=(PFIFO)arg;
  
  for(i=0;i<N_PRODUCERS*MAX_PRODUCTS;i++)
  {                     
    sleep(1);
    sem_wait(&pFifo->sC);
    sem_wait(&pFifo->sMe);
    f=pFifo->buffer[pFifo->tail];
    pFifo->tail=(pFifo->tail+1)%MAX_BUFFER_SIZE;
    pFifo->numElems--;
    sem_post(&pFifo->sP);
    sem_post(&pFifo->sMe);
    printf("Consuming %f\n",f);
  }
  return NULL;
}

void initFIFO(PFIFO pFifo)
{
  pFifo->head=pFifo->tail=pFifo->numElems=0;
  sem_init(&pFifo->sMe, 0, 1);
  sem_init(&pFifo->sP, 0, MAX_BUFFER_SIZE);
  sem_init(&pFifo->sC, 0, 0);
}

void cleanFIFO(PFIFO pFifo)
{
  sem_destroy(&pFifo->sMe);
  sem_destroy(&pFifo->sP);
  sem_destroy(&pFifo->sC);
}
