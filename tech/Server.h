#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <vector>
#include "SocketRAII.h"

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::runtime_error;

using socket_raii::SocketRAII;
using exceptions::WinSockErrorException;

namespace server {

	const PCSTR DEFAULT_PORT = "12345";

	class Server {
	public:
		/**
		 * @brief Initializes the WinSock library
		 * @throws WinSockErrorException if initialization fails
		 */
		Server();
		~Server();

		void listenForClients();
		SocketRAII acceptClient();
		void handleClient(SocketRAII& clientSocket);

		void start();

	private:
		SocketRAII m_listenSocket;

		void handleCommand(SocketRAII& clientSocket, const string& commandString);
		void unknownCommand(SocketRAII& clientSocket);

		void handlePing(SocketRAII& clientSocket);

		using CommandHandler = void (Server::*)(SocketRAII&);
		map<string, CommandHandler> commandToHandler;

	};

}  // namespace server
