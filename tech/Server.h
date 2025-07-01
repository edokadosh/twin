#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;

namespace server {

<<<<<<< HEAD
	const size_t DEFAULT_BUFLEN = 512;
	const PCSTR DEFAULT_PORT = "12345";

	class Server {
	private:
		SOCKET m_listenSocket = INVALID_SOCKET;
		vector<SOCKET> m_clientSockets;

		void removeClient(SOCKET clientSocket);

		void handlePing(SOCKET clientSocket);

		map<string, void (Server::*)(SOCKET)> commandToHandler;

	public:
		Server();
		~Server();


		void listen();
		SOCKET acceptClient();
		void handleClietn(SOCKET clientSocket);

		void start();
	};

=======
const size_t DEFAULT_BUFLEN = 512;
const PCSTR DEFAULT_PORT = "12345";

class Server {
 private:
  SOCKET m_listenSocket = INVALID_SOCKET;
  vector<SOCKET> m_clientSockets;

  void removeClient(SOCKET clientSocket);

  void handlePing(SOCKET clientSocket);

  static const map<string, void (*)(SOCKET)> commandToHandler;
 public:
  Server();
  ~Server();


  void listen();
  SOCKET acceptClient();
  void handleClietn(SOCKET clientSocket);

  void start();
};

const map<string, void (*)(SOCKET)> Server::commandToHandler = {
    { 'PING', handlePing }
};
>>>>>>> 230e36c77ca2b9d5e431a19e4cf4481f5ab581f4

}  // namespace server
