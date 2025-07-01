#include "Server.h"

namespace server {
	WinSockErrorException::WinSockErrorException(const string msg) : message(msg) {
		// Left blank intentionally
	}
	const char* WinSockErrorException::what() const noexcept {
		return message.c_str();
	}

	Server::Server() {
		commandToHandler = {
			{ string("PING"), &Server::handlePing }
		};
	}

	Server::~Server() {
		WSACleanup();
		closesocket(m_listenSocket);
		for (auto s : m_clientSockets) {
			closesocket(s);
		}
	}

	void Server::listenForClients() {
		WSADATA wsaData;
		int iResult;

		struct addrinfo* result = NULL;
		struct addrinfo hints;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			throw WinSockErrorException("WSAStartup failed");
		}

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
		if (iResult != 0) {
			printf("getaddrinfo failed with error: %d\n", iResult);
			throw WinSockErrorException("getaddrinfo failed");
		}

		// Create a SOCKET for the server to listen for client connections.
		m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (m_listenSocket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			throw WinSockErrorException("socket failed");
		}

		// Setup the TCP listening socket
		iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			throw WinSockErrorException("bind failed");
		}

		freeaddrinfo(result);

		iResult = listen(m_listenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR) {
			printf("listen failed with error: %d\n", WSAGetLastError());

			throw WinSockErrorException("listen failed");
		}
	}

	SOCKET Server::acceptClient() {
		SOCKET clientSocket = accept(m_listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			throw WinSockErrorException("accept failed");
		}
		m_clientSockets.push_back(clientSocket);
		return clientSocket;
	}

	void Server::removeClient(SOCKET clientSocket) {
		m_clientSockets.erase(
			std::remove(m_clientSockets.begin(), m_clientSockets.end(), clientSocket),
			m_clientSockets.end());
		closesocket(clientSocket);
	}

	void Server::handlePing(SOCKET clientSocket) {
		char ping_response[] = "PONG";
		int sent = send(clientSocket, ping_response, strlen(ping_response), 0);
		if (sent == SOCKET_ERROR) {
			printf("send failed with error: %d\n", WSAGetLastError());
			removeClient(clientSocket);
			throw WinSockErrorException("send failed");
		}
		printf("Bytes sent: %d\n", sent);
	}

	void Server::handleClietn(SOCKET clientSocket) {
		int sent;
		char recvbuf[DEFAULT_BUFLEN + 1];

		do {
			sent = recv(clientSocket, recvbuf, DEFAULT_BUFLEN, 0);

			if (sent < 0) {
				removeClient(clientSocket);
				throw WinSockErrorException("recv failed");
			}
			else if (sent == 0) {
				cout << "Connection closing..." << endl;
				continue;
			}
			recvbuf[DEFAULT_BUFLEN] = '\0';
			string commandString(recvbuf);

			if (commandToHandler.find(commandString) == commandToHandler.end()) {
				cout << "Got Unknown command" << endl;
			}
			else {
				((*this).*commandToHandler[commandString])(clientSocket);
			}

		} while (sent > 0);

		removeClient(clientSocket);
	}

	void Server::start() {
		listenForClients();
		SOCKET clientSocket = acceptClient();

		handleClietn(clientSocket);
	}

} // namespace server
