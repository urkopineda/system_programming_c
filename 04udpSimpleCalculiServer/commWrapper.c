#include "commWrapper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int remoteGCD(int sock, struct sockaddr_in *pAddr, uint32_t n, uint32_t m, uint32_t *pResult) {
  char str[32];
  int i;
  sprintf(str,"gcd %u %u",n,m);
  if(-1==sendto(sock,str,strlen(str),0,(struct sockaddr*)pAddr,sizeof(*pAddr))) return -1;
  if((i=recvfrom(sock,str,32,0,NULL,NULL))==-1) return -1;
  str[i]='\0';
  sscanf(str, "%d",pResult);
  return 0;
}

int remoteLCM(int sock, struct sockaddr_in *pAddr, uint32_t m, uint32_t n, uint32_t *pResult) {
  char str[32];
  int i;
  sprintf(str,"lcm %u %u",n,m);
  if(-1==sendto(sock,str,strlen(str),0,(struct sockaddr*)pAddr,sizeof(*pAddr))) return -1;
  if((i=recvfrom(sock,str,32,0,NULL,NULL))==-1) return -1;
  str[i]='\0';
  sscanf(str, "%d",pResult);
  return 0;
}

int waitPetition(int sock, struct sockaddr_in* fromWhere, PMESSAGE pMessage) {
  char str[128], strMessage[16];
  socklen_t sizeAddr;
  int i;
  sizeAddr=sizeof(struct sockaddr_in);
  if(-1 == (i=recvfrom(sock,str,128,0,(struct sockaddr*) fromWhere, &sizeAddr))) {
    return -1;
  }
  str[i]='\0';
  sscanf(str,"%s%u%u",strMessage, &(pMessage->param1), &(pMessage->param2)); 
  if(strcmp(strMessage,"lcm")==0) pMessage->type=LCM;
  else pMessage->type=GCD;
  return 0;
}

int sendAnswer(int sock, const struct sockaddr_in* to, uint32_t value) {
  char str[32];
  sprintf(str,"%u",value);
  if(-1 == sendto(sock,str,strlen(str),0,(struct sockaddr*)to, sizeof(*to))) return -1;
  return 0;
}

int initializeListeningPort(int *pSock, char *strIP, int port) {
  struct sockaddr_in hereAddr;
  if((*pSock=socket(PF_INET,SOCK_DGRAM,0))==-1) {
    perror("socket\n");
    exit(-1);
  }
  hereAddr.sin_family=AF_INET;
  hereAddr.sin_addr.s_addr=inet_addr(strIP);
  hereAddr.sin_port=htons(port);
  if(bind(*pSock,(struct sockaddr*)&hereAddr,sizeof(hereAddr))==-1) return -1;
  else return 0;
}

