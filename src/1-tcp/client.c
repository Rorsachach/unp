#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/utils.h"

void str_cli(FILE*, int);

int main(int argc, char** argv) {
  int sockfd; // 1. socket fd
  struct sockaddr_in addr; // 2. server addr

  if (argc != 2) {
    perror("[Usage: client <server-ip>]");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("create socket error");
    exit(1);
  }
  
  bzero(&addr, sizeof (addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(55555);
  inet_pton(AF_INET, argv[1], &addr.sin_addr);

  connect(sockfd, (struct sockaddr*) &addr, sizeof(addr));
  
  printf("connect\n");

  /* 数据通信 */
  str_cli(stdin, sockfd);

  exit(0);
}

void str_cli(FILE* file, int sockfd) {
  char sendline[BUFFER_MAX], recvline[BUFFER_MAX];

  while (fgets(sendline, BUFFER_MAX, file) != NULL) {
    writen(sockfd, sendline, strlen(sendline));

    if (readline(sockfd, recvline, BUFFER_MAX) == 0) {
      perror("server teminated prematuraly");
      exit(1);
    }
    fputs(recvline, stdout);
  }
}

