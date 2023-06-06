#include <sys/select.h>
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
  int nfd;
  fd_set rset;
  char sendline[BUFFER_MAX], recvline[BUFFER_MAX];

  int file_eof = 0;

  FD_ZERO(&rset);

  for (;;) {
    if (file_eof == 0)
      FD_SET(fileno(file), &rset);
    FD_SET(sockfd, &rset);
    nfd = fileno(file) > sockfd ? fileno(file) + 1 : sockfd + 1;

    select(nfd, &rset, NULL, NULL, NULL);

    if (FD_ISSET(fileno(file), &rset)) {
      if (fgets(sendline, BUFFER_MAX, file) == NULL) {
        file_eof = 1;
        shutdown(sockfd, SHUT_WR);
        FD_CLR(fileno(file), &rset);
        continue;
      }
      else writen(sockfd, sendline, strlen(sendline));
    }

    if (FD_ISSET(sockfd, &rset)) {
      if (readline(sockfd, recvline, BUFFER_MAX) == 0) {
        if (file_eof == 1) {
          printf("[info]: normal terminated.\n");
          exit(0);
        } else {
          perror("server terminated prematuraly");
          exit(1);
        }
      } else fputs(recvline, stdout);
    }
  }
}

