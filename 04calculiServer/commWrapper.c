#include "commWrapper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct sockaddr_in sAddr;

int waitFrame(int sock, char str[], int maxSize);

int waitFrame(int sock, char str[], int maxSize) {
  int cnt = 0, i;
  int sockLen=sizeof(sAddr);
  fflush(stdout);
  do {
    i = recvfrom(sock, str + cnt, maxSize - cnt, 0, (struct sockaddr *) &sAddr, sockLen);
    if ((i == 0) || (i == -1)) return -1;
    cnt += i;
  } while (str[cnt-1] != '$');
  str[cnt-1]='\0';
  return 0;
}

int waitMessage(int sock, PMESSAGE pMessage) {
  char str[128], strMessage[16];
  if (waitFrame(sock, str, 128)==-1) return -1;
  sscanf(str,"%s%u%u",strMessage, &(pMessage->param1), &(pMessage->param2)); 
  if (strcmp(strMessage,"lcm")==0) pMessage->type=LCM;
  else pMessage->type=GCD;
  return 0;
}

int waitAnswer(int sock, uint32_t *pValue) {
  char str[128];
  waitFrame(sock, str, 128);
  sscanf(str,"%u", pValue);
  return 0;
}

int sendAnswer(int sock, uint32_t value) {
  char str[32];
  sprintf(str,"%u$",value);
  int sockLen=sizeof(sAddr);
  sendto(sock,str,strlen(str),0,(struct sockaddr *) &sAddr, sockLen);
  return 0;
}

int remoteGCD(int sock, uint32_t n, uint32_t m, uint32_t *pResult) {
  char str[32];
  sprintf(str,"gcd %u %u$",n,m);
  int sockLen=sizeof(sAddr);
  sendto(sock,str,strlen(str),0, (struct sockaddr *) &sAddr, sockLen);
  waitFrame(sock, str,32);
  sscanf(str, "%d",pResult);
  return 0;
}

int remoteLCM(int sock, uint32_t n, uint32_t m, uint32_t *pResult) {
  char str[32];
  sprintf(str,"lcm %u %u$",n,m);
  int sockLen=sizeof(sAddr);
  sendto(sock,str,strlen(str),0, (struct sockaddr *) &sAddr, sockLen);
  waitFrame(sock, str, 32);
  sscanf(str, "%u", pResult);
  return 0;
}
int connectToServer(int *pSockConnection, int serverPort) {
  struct sockaddr_in sAddr;

  if((*pSockConnection=socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP))==-1)
  {
    return -1;
  }
  sAddr.sin_family=AF_INET;
  sAddr.sin_port=htons(serverPort);
  if(connect(*pSockConnection,(struct sockaddr*)&sAddr,sizeof(sAddr))==-1)
  {
     close(*pSockConnection);
     return -1;
  }
  return 0;
}
int initServerSocket(int *pServerSock, int serverPort) {
  if((*pServerSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("error creating the socket\n");
    return -1;
  }
  sAddr.sin_family=AF_INET;
  sAddr.sin_addr.s_addr=htonl(INADDR_ANY);
  sAddr.sin_port=htons(serverPort);
  if(bind(*pServerSock, (struct sockaddr*)&sAddr,sizeof(sAddr))==-1) {
    perror("I coundn't bind\n");
    return -1;
  }
  return 0;
}
