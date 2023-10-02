#include <iostream>
#include "../network.h"

int main(int argc, char *argv[]) {
  _sock_init();
  SOCKET socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(2233);
  ::inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);
  if (::connect(socket, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    return -1;
  }
  std::string msg = "ping";
  ::send(socket, msg.c_str(), msg.length(), 0);

  std::cout << "::send." << msg.c_str() << std::endl;

  char buffer[1024];
  memset(&buffer, 0, sizeof(buffer));
  ::recv(socket, buffer, sizeof(buffer), 0);
  std::cout << "::recv." << buffer << std::endl;

  _sock_close(socket);
  _sock_exit();
  return 0;
}