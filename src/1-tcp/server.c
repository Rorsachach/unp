#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/utils.h"

void str_echo(int);

int main(int argc, char** argv) {
  int serverSockFd, clientSockFd;
  struct sockaddr_in serverAddr, clientAddr;
  socklen_t client_length;
  pid_t cpid;

  if ((serverSockFd = socket(AF_INET, SOCK_STREAM, 0)) == - 1) {
    perror("create socket error");
    exit(1);
  }

  bzero(&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(55555);
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  bind(serverSockFd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

  listen(serverSockFd, 5);

  for (;;) {
    client_length = sizeof(clientAddr);
    clientSockFd = accept(serverSockFd, (struct sockaddr*) &clientAddr, &client_length);
    if ((cpid = fork()) == 0) {
      close(serverSockFd);
      /* 数据通信 */
      str_echo(clientSockFd);
      exit(0);
    }
    close(clientSockFd);
  }

  exit(0);
}


void str_echo(int sockfd) {
  ssize_t n;
  char buf[1024];

again:
  while ((n = readn(sockfd, buf, 1024)) > 0) {
    writen(sockfd, buf, n);
  }

  if (n < 0 && errno == EINTR)
    goto again;
  else if (n < 0)
    perror("[Error]: read error.");
}
