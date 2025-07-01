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

	// Exception class forserver errors
	class WinSockErrorException : public std::exception {
	private:
		string message;
	public:
		WinSockErrorException(const string msg);
		const char* what() const noexcept;
	};

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


		void listenForClients();
		SOCKET acceptClient();
		void handleClietn(SOCKET clientSocket);

		void start();
	};

}  // namespace server
