#pragma once

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define BUFFER_MAX 4096

ssize_t readn(int fd, void* vptr, size_t n) {
  ssize_t nread;

  size_t nleft = n;
  char* ptr = (char *) vptr;

  while (nleft > 0) {
    if ((nread = read(fd, ptr, nleft)) < 0) {
      if (errno == EINTR) {
        nread = 0;
      } else {
        return -1;
      }
    } else if (nread == 0)
      break;

    nleft -= nread;
    ptr += nread;
  }

  return (n - nleft);
}

ssize_t writen(int fd, void* vptr, size_t n) {
  ssize_t nwrite;

  size_t nleft = n;
  char* ptr = (char *) vptr;

  while (nleft > 0) {
    if ((nwrite = write(fd, ptr, nleft)) < 0) {
      if (errno == EINTR) nwrite = 0;
      else return -1;
    }

    nleft -= nwrite;
    ptr += nwrite;
  }

  return n;
}

ssize_t readline(int fd, void* vptr, int maxlen) {
  ssize_t n, rc;
  char c;
  char* ptr = (char *) vptr;

  for (n = 1; n < maxlen; ++n) {
again:
    if ((rc = read(fd, &c, 1)) == 1) {
      *ptr++ = c;
      if (c == '\n')
        break;
    } else if (rc == 0) {
      *ptr = 0;
      return (n - 1);
    } else {
      if (errno == EINTR) goto again;
      return -1;
    }
  }

  *ptr = 0;
  return n;
}
