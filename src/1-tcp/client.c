#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  int sockfd; // 1. socket fd
  struct sockaddr_in addr; // 2. server addr

  if (argc != 2) {
    perror("[Usage: client <server-ip>]");
    exit(1);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    perror("create socket error");
  }
  

  exit(0);
}
