#include "commWrapper.h"
#include "functions01.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include "communicationMonitor.h"

#define N_WORKERS 2

void sigHandler();

void checkArgs(int argc, char* argv []);
void attendClient(int sock);
void* returnWaitingThreadEntry(void* arg);
void* workerThreadEntry(void* arg);

int serverSock;
COMMUNICATION_MONITOR commonZone;

int main (int argc, char* argv [])
{
  int sockConnection, port, i;
  char str[128];
  pthread_t workersPool[N_WORKERS];
   
  checkArgs(argc, argv);
  init(&commonZone,N_WORKERS);
  sscanf(argv[2],"%d", &port);
  signal(SIGINT,sigHandler);
  for(i=0;i<N_WORKERS;i++)
    pthread_create(&workersPool[i], NULL, workerThreadEntry, (void *) &commonZone);
  if(initServerSocket(&serverSock, argv[1], port)!=0)
  {
    sprintf(str,"I can't use %s:%d\n",argv[1],port);
    perror(str);
    return -1;
  }
  while(!waitIddleTask(&commonZone))
  {
    if(waitConexion(serverSock,&sockConnection)!=-1)
    {
      put(&commonZone, sockConnection);
    }
    else interrupt(&commonZone);
  }
  for(i=0;i<N_WORKERS;i++)
    pthread_join(workersPool[i], NULL);
  return 0;
}

void attendClient(int sock)
{
  MESSAGE msg;

  while(-1 != waitMessage(sock, &msg))
  {
    switch(msg.type)
    {
      case GCD: sendAnswer(sock,gcd(msg.param1,msg.param2));
                break;
      case LCM: sendAnswer(sock,lcm(msg.param1,msg.param2));
                break;
    }
  }
  close(sock);
}

void* workerThreadEntry(void* arg)
{
  PCOMMUNICATION_MONITOR pMon = (PCOMMUNICATION_MONITOR) arg;
  int sock;

  while(!get(pMon, &sock))
  {
    attendClient(sock);
    addIddleTask(pMon);
  }
  return NULL;
}

void checkArgs(int argc, char* argv [])
{
  if(argc!=3)
  {
    printf("invocation error in program invocation. Correct format:\n ");
    printf("%s <serverIP> <serverPort>\n ", argv[0]);
    exit(-1);
  }
}

void sigHandler()
{
 signal(SIGINT,SIG_IGN);
 printf("Preparing to close. No more clients will be accepted\n");
 close(serverSock);
 signal(SIGINT,sigHandler);
}
