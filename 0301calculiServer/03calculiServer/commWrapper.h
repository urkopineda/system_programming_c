#ifndef COMMWRAPPER_H
#define COMMWRAPPER_H
#include <stdint.h>

#define LCM 1
#define GCD 2
#define ANSWER 3

typedef struct s_Message
{
  int type;
  uint32_t param1;
  uint32_t param2;
} MESSAGE, *PMESSAGE;

int connectToServer(int *pSockConnection, char* serverIP, int serverPort);
int initServerSocket(int *pServerSock, char* serverIP, int serverPort);
int waitConexion(int serverSock, int *psockConnection);
int waitMessage(int sock, PMESSAGE pMessage);
int sendAnswer(int sock, uint32_t);
int remoteGCD(int sock, uint32_t n, uint32_t m, uint32_t *pResult);
int remoteLCM(int sock, uint32_t n, uint32_t m, uint32_t *pResult);

#endif
