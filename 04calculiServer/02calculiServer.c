#include "commWrapper.h"
#include "functions01.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

void attendClient(int sock);

int serverSock;

int main (int argc, char* argv []) {
  int sockConnection, port;
  char str[128];
  sscanf(argv[1],"%d", &port);
  if (initServerSocket(&serverSock, port) != 0) {
    sprintf(str,"I can't use %d\n",port);
    perror(str);
    return -1;
  }
  while (1) {
    attendClient(sockConnection);
  }
  return 0;
}

void attendClient(int sock) {
  MESSAGE msg;
  while(-1 != waitMessage(sock, &msg)) {
    switch(msg.type) {
      case GCD: sendAnswer(sock,gcd(msg.param1,msg.param2));
                break;
      case LCM: sendAnswer(sock,lcm(msg.param1,msg.param2));
                break;
    }
  }
  close(sock);
}
