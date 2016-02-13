#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "commWrapper.h"

void checkArgs(int argc, char* argv []);

int main(int argc,char** argv) {
  int sock, port;
  uint32_t n1,n2, result;
  struct sockaddr_in sAddr;
  char str01[32], str02[32];
  checkArgs(argc, argv);
  sscanf(argv[2],"%d", &port);
  if((sock=socket(PF_INET,SOCK_DGRAM,0))==-1) {
    perror("socket\n");
    exit(-1);
  }
  memset((char*)&sAddr,0,sizeof(sAddr));
  sAddr.sin_family=AF_INET;
  sAddr.sin_addr.s_addr=inet_addr(argv[1]);
  sAddr.sin_port=htons(port);
  printf ("give me a natural number: ");
  fgets (str01,32,stdin);
  sscanf(str01,"%u",&n1);
  printf ("give me a natural number: ");
  fgets(str02,32,stdin);
  sscanf(str02,"%u",&n2);
  remoteLCM(sock, &sAddr, n1, n2, &result);
  printf("lcm(%u,%u)=%u\n", n1, n2, result);
  remoteGCD(sock, &sAddr, n1, n2, &result);
  printf("gcd(%u,%u)=%u\n", n1, n2, result);
  close(sock);
  close(sock);
  exit(0);
}

void checkArgs(int argc, char* argv []) {
  if(argc!=3) {
    printf("invocation error in program invocation. Correct format:\n ");
    printf("%s <serverIP> <serverPort>\n ", argv[0]);
    exit(-1);
  }
}
