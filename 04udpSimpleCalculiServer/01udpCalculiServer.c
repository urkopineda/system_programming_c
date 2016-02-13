#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include "commWrapper.h"
#include "functions01.h"

void checkArgs(int argc, char* argv []);
void sigHandler();
int attendPetition(int sock,  const struct sockaddr_in* fromWhere, const PMESSAGE  pMessage);

int sock;

int main(int argc,char** argv) {
  int  port;
  struct sockaddr_in thereAddr;
  MESSAGE msg;
  checkArgs(argc, argv);
  signal(SIGINT,sigHandler);
  sscanf(argv[2],"%d", &port);
  initializeListeningPort(&sock, argv[1], port);
  while(-1 != waitPetition(sock, &thereAddr, &msg)) {
    msg->sock = sock;
    msg->sockaddr_in = &thereAddr;
    
    if(-1 == attendPetition(sock, &thereAddr, &msg)) printf("client not correctly attended\n");
  }
  return 0;
}

int attendPetition(int sock, const struct sockaddr_in* to, const PMESSAGE pMsg) {
  // PRODUCIR --> PASAR VARIABLES A PRODUCTOR
  // CREAR HILO CONSUMIDOR
  // INICIAR HILO CONSUMIDOR
  return 0;
}

void sigHandler() {
 signal(SIGINT,SIG_IGN);
 printf("Preparing to close. No more clients will be accepted\n");
 close(sock);
 signal(SIGINT,sigHandler);
}

void checkArgs(int argc, char* argv []) {
  if(argc!=3) {
    printf("invocation error in program invocation. Correct format:\n ");
    printf("%s <serverIP> <serverPort>\n ", argv[0]);
    exit(-1);
  }
}
