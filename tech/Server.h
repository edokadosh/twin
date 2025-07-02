#pragma once

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <vector>

#include "SocketGuard.h"

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;
using std::runtime_error;

using socket_guard::SocketGuard;

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
		SocketGuard acceptClient();
		void handleClient(SocketGuard& clientSocket);

		void start();

	private:
		SocketGuard m_listenSocket;

		void handleCommand(SocketGuard& clientSocket, const string& commandString);
		void unknownCommand(SocketGuard& clientSocket);

		void handlePing(SocketGuard& clientSocket);

		using CommandHandler = void (Server::*)(SocketGuard&);
		map<string, CommandHandler> commandToHandler;

	};

}  // namespace server
