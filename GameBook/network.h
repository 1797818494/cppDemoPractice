#pragma once
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SOCKET int
#define INVALID_SOCKET -1
#define _sock_init()
#define _sock_exit()
#define _sock_err() errno
#define _sock_close(sockfd) ::close(sockfd)
inline int GetListenBacklog() {
  int backlog = 10;
  char *ptr;
  if ((ptr = getenv("LISTENQ")) != nullptr) {
    backlog = atoi(ptr);
  }
  return backlog;
}