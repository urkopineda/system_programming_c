#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* producersFunction(void*);
void* consumersFunction(void*);

#define MAX_BUFFER_SIZE 5
#define MAX_PRODUCTS 6

typedef struct s_Fifo
{
  int head;
  int tail;
  int isFull;
  int isEmpty;
  char buffer[MAX_BUFFER_SIZE];
  pthread_mutex_t mtx;
  pthread_cond_t condNotFull;
  pthread_cond_t condNotEmpty;
}FIFO,*PFIFO;


FIFO fifo={0,0,0,1};

int main()
{
  pthread_t idProducer,idConsumer;
  char str[32];

  pthread_mutex_init(&fifo.mtx,NULL);
  pthread_cond_init(&fifo.condNotFull,NULL);
  pthread_cond_init(&fifo.condNotEmpty,NULL);
  pthread_create(&idProducer,NULL,producersFunction,NULL);
  pthread_create(&idConsumer,NULL,consumersFunction,NULL);
  pthread_join(idProducer,NULL);
  pthread_join(idConsumer,NULL);
  pthread_mutex_destroy(&fifo.mtx);
  pthread_cond_destroy(&fifo.condNotFull);
  pthread_cond_destroy(&fifo.condNotEmpty);
  printf("type \"Return\" to finish\n");
  fgets(str,sizeof(str),stdin); 
  return 0;
}

void* producersFunction(void* arg)
{
  int i=0,wasEmpty;

  for(i=0;i<MAX_PRODUCTS;i++)
  {
    //sleep(1);
    pthread_mutex_lock(&fifo.mtx);
    while(fifo.isFull) pthread_cond_wait(&fifo.condNotFull,&fifo.mtx);
    wasEmpty=fifo.isEmpty;
    fifo.buffer[fifo.head]=(i+1)*15;
    fifo.head=(fifo.head+1)%MAX_BUFFER_SIZE;
    printf("PRODUCER. head=%d tail=%d\n",fifo.head,fifo.tail);
    fifo.isEmpty=0;
    fifo.isFull=(fifo.tail==fifo.head);
    if(wasEmpty) pthread_cond_signal(&fifo.condNotEmpty);
    pthread_mutex_unlock(&fifo.mtx);
  }
  return NULL;
}

void* consumersFunction(void* arg)
{
  int n,i,wasFull;
  
  for(i=0;i<MAX_PRODUCTS;i++)
  {
	sleep(1);
    pthread_mutex_lock(&fifo.mtx);
    while(fifo.isEmpty) pthread_cond_wait(&fifo.condNotEmpty,&fifo.mtx);
    wasFull=fifo.isFull;
    n=fifo.buffer[fifo.tail];
    fifo.tail=(fifo.tail+1)%MAX_BUFFER_SIZE;
    printf("%d\n",n);
    printf("CONSUMER. head=%d tail=%d\n",fifo.head,fifo.tail);
    fifo.isEmpty=(fifo.head==fifo.tail);
    fifo.isFull=0;
    if(wasFull) pthread_cond_broadcast(&fifo.condNotFull);
    sleep(1);
    pthread_mutex_unlock(&fifo.mtx);
  }
  return NULL;
}
