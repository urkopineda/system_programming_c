#include "commWrapper.h"
#include "functions01.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

void sigHandler();

void checkArgs(int argc, char* argv []);
void attendClient(int sock);

void *attendClientThread(void *arg);

int serverSock;
int count;

pthread_t threadArray [50];

int main (int argc, char* argv [])
{
  int sockConnection, port;
  char str[128];
  count = 0;
  
  checkArgs(argc, argv);
  sscanf(argv[2],"%d", &port);
  signal(SIGINT,sigHandler);
  if(initServerSocket(&serverSock, argv[1], port)!=0)
  {
    sprintf(str,"I can't use %s:%d\n",argv[1],port);
    perror(str);
    return -1;
  }
  if (count < 50) {
	  while(waitConexion(serverSock,&sockConnection)!=-1)
	  {
	  	pthread_create(&threadArray[count], NULL, attendClientThread, (void *) sockConnection);
	  	count++;
	  }
  } else {
  	printf("Server busy, aborting...\n");
  	return -1;
  }
  return 0;
}

void *attendClientThread(void *arg) {
	attendClient((int) arg);
	return NULL;
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
 printf("closing server socket\n");
 close(serverSock);
 signal(SIGINT,sigHandler);
}
