#ifndef COMMWRAPPER_H
#define COMMWRAPPER_H
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LCM 1
#define GCD 2
#define ANSWER 3

typedef struct s_Message {
  int type;
  uint32_t param1;
  uint32_t param2;
  struct sockaddr_in;
  int sock;
} MESSAGE, *PMESSAGE;

int sendAnswer(int sock, const struct sockaddr_in* to, uint32_t);
int remoteGCD(int sock, struct sockaddr_in  *pAddr, uint32_t n, uint32_t m, uint32_t *pResult);
int remoteLCM(int sock, struct sockaddr_in *pAddr, uint32_t m, uint32_t n, uint32_t *pResult);
int initializeListeningPort(int *pPort, char *strIP, int port);
int waitPetition(int sock, struct sockaddr_in* fromWhere, PMESSAGE pMessage);

#endif
