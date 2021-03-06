#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 5000

int main(int argc,char** argv)
{
  int sock,i;
  struct sockaddr_in sAddr;
  char str[128];

  if((sock=socket(PF_INET,SOCK_STREAM,0))==-1)
  {
    printf("error al crear el socket\n");
    exit(-1);
  }
  sAddr.sin_family=AF_INET;
  sAddr.sin_addr.s_addr=inet_addr(SERVER_IP );
  sAddr.sin_port=htons(SERVER_PORT);
  if(connect(sock,(struct sockaddr*)&sAddr,sizeof(sAddr))==-1)
  {
     perror("connect\n");
     exit(-1);
  }
  printf("What do you whant to send? ");
  gets(str);
  send(sock,str,strlen(str),0);
  i=recv(sock,str,128,0);
  str[i]='\0';
  printf("RESPONSE: \"%s\"\n",str);
  close(sock);
  return 0;
}
