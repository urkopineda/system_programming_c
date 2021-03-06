#include "commWrapper.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void checkArgs(int argc, char* argv []);

int main (int argc, char* argv [])
{
  char str01[32], str02[32];
  unsigned int  n1, n2 , result01, result02;
  int sock, port;

  checkArgs(argc, argv);
  sscanf(argv[2],"%d", &port);
  if(connectToServer(&sock, argv[1], port)!=0)
  {
    sprintf(str01,"I couldn't connect to %s:%d\n",argv[1],port);
    perror(str01);
    return -1;
  }
  printf ("give me a natural number: ");
  fgets (str01,32,stdin);
  sscanf(str01,"%u",&n1);
  printf ("give me a natural number: ");
  fgets(str02,32,stdin);
  sscanf(str02,"%u",&n2);
  remoteGCD(sock, n1, n2, &result01);
  printf("gcd(%u,%u)=%u\n", n1, n2, result01);
  remoteLCM(sock,n1, n2, &result02);
  printf("lcm(%u,%u)=%u\n", n1, n2, result02);
  close(sock);
  return 0;
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
