#include "Server.h"
#include "utils.h"
#include "exceptions.h"
#include "AddrInfoGuard.h"

using exceptions::printError;
using exceptions::WinSockErrorException;
using exceptions::checkWinSockError;
using exceptions::checkError;

using addrinfo_guard::AddrInfoGuard;

namespace server {
	

	Server::Server() {
		WSADATA wsaData;

		// Initialize Winsock
		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0) {
			cout << "WSAStartup failed with error: " << result << endl;
			throw WinSockErrorException("WSAStartup failed");
		}

		commandToHandler = {
			{ string("PING"), &Server::handlePing }
		};
	}

	Server::~Server() {
		WSACleanup();
	}

	void Server::listenForClients() {
		struct addrinfo hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		AddrInfoGuard addrInfo(NULL, DEFAULT_PORT, &hints);

		m_listenSocket = SocketGuard(socket(addrInfo.get()->ai_family, addrInfo.get()->ai_socktype, addrInfo.get()->ai_protocol));

		// Setup the TCP listening socket
		checkWinSockError(bind(m_listenSocket.get(), addrInfo.get()->ai_addr, (int)addrInfo.get()->ai_addrlen), "bind");

		checkWinSockError(listen(m_listenSocket.get(), SOMAXCONN), "listen");

		cout << "Listening for connections on port: " << DEFAULT_PORT << endl;
	}

	SocketGuard Server::acceptClient() {
		return SocketGuard(accept(m_listenSocket.get(), NULL, NULL));
	}


	void Server::handleCommand(SocketGuard& clientSocket, const string& commandString) {
		if (commandToHandler.find(commandString) == commandToHandler.end()) {
			unknownCommand(clientSocket);
		}
		else {
			CommandHandler handler = commandToHandler[commandString];
			(this->*handler)(clientSocket);
			
		}
	}

	void Server::unknownCommand(SocketGuard& clientSocket) {
		string unknown_response = "UNKNOWN COMMAND";
		clientSocket.send(unknown_response);
	}

	void Server::handlePing(SocketGuard& clientSocket) {
		string ping_response = "PONG";
		int sent = clientSocket.send(ping_response);
	}

	void Server::handleClient(SocketGuard& clientSocket) {
		string clientMessage = "";

		do {
			clientMessage = clientSocket.recv();

			if (clientMessage == "") {
				cout << "Connection closing..." << endl;
				continue;
			}
			
			handleCommand(clientSocket, clientMessage);
		} while (clientMessage != "");
	}

	void Server::start() {
		listenForClients();
		SocketGuard clientSocket = acceptClient();

		handleClient(clientSocket);
	}

} // namespace server
