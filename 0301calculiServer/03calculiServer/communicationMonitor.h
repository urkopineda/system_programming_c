#ifndef COMMUNICATIONMONITOR_H
#define COMMUNICATIONMONITOR_H

#include <pthread.h>

#define MAX_BUFFER_SIZE 2

typedef struct s_CommunicationMonitor
{
  int head;
  int tail;
  int numElems;
  int buffer[MAX_BUFFER_SIZE]; //for socket handlers
  pthread_mutex_t mtx;
  pthread_cond_t condNotFull;
  pthread_cond_t condNotEmpty;
  pthread_cond_t condSomeIddleTask;
  int nIddleTasks;
  int goOut;
}COMMUNICATION_MONITOR,*PCOMMUNICATION_MONITOR;

void init(PCOMMUNICATION_MONITOR pMon, int initialIddleTasks);
int put(PCOMMUNICATION_MONITOR pMon, int sock);
int get(PCOMMUNICATION_MONITOR pMon, int * pSock);
void interrupt(PCOMMUNICATION_MONITOR pMon);
void destroy(PCOMMUNICATION_MONITOR pMon);
int waitIddleTask(PCOMMUNICATION_MONITOR pMon);
void addIddleTask(PCOMMUNICATION_MONITOR pMon);

#endif

