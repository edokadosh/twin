#pragma once

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

namespace server {

const size_t DEFAULT_BUFLEN = 512;
const std::string DEFAULT_PORT = "27015";

class Server {
 private:
  SOCKET m_listenSocket;
  SOCKET m_clientSocket;


 public:
  Server();

  void listen();
};

}  // namespace server
