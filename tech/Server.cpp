#include "Server.h"
#include "exceptions.h"
#include "AddrInfoRAII.h"

using exceptions::printError;
using exceptions::WinSockErrorException;
using exceptions::checkWinSockError;
using exceptions::checkError;

using addrinfo_raii::AddrInfoRAII;


namespace server {

	vector<string> split(const string& str, char delimiter) {
		vector<string> tokens;
		size_t start = 0;
		size_t end = str.find(delimiter);

		while (end != string::npos) {
			tokens.push_back(str.substr(start, end - start));
			start = end + 1;
			end = str.find(delimiter, start);
		}
		tokens.push_back(str.substr(start));

		return tokens;
	}

	string join(const vector<string>& tokens, char delimiter) {
		string result;
		for (auto it : tokens) {
			if (!result.empty()) {
				result += delimiter;
			}
			result += it;
		}
		return result;
	}
	

	Server::Server() {
		WSADATA wsaData;

		// Initialize Winsock
		int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (result != 0) {
			cout << "WSAStartup failed with error: " << result << endl;
			throw WinSockErrorException("WSAStartup failed");
		}

		commandToHandler = {
			{ "PING", &Server::handlePing },
			{ "RUN", &Server::handleRun }
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

		AddrInfoRAII addrInfo(NULL, DEFAULT_PORT, &hints);

		m_listenSocket = SocketRAII(socket(addrInfo.get()->ai_family, addrInfo.get()->ai_socktype, addrInfo.get()->ai_protocol));

		// Setup the TCP listening socket
		checkWinSockError(bind(m_listenSocket.get(), addrInfo.get()->ai_addr, (int)addrInfo.get()->ai_addrlen), "bind");

		checkWinSockError(listen(m_listenSocket.get(), SOMAXCONN), "listen");

		cout << "Listening for connections on port: " << DEFAULT_PORT << endl;
	}

	SocketRAII Server::acceptClient() {
		return SocketRAII(accept(m_listenSocket.get(), NULL, NULL));
	}


	void Server::handleCommand(SocketRAII& clientSocket, const string& commandString) {
		vector<string> tokens = split(commandString, ' ');
		if (tokens.empty()) {
			unknownCommand(clientSocket);
			return;
		}
		string command = tokens[0];

		if (commandToHandler.find(command) == commandToHandler.end()) {
			unknownCommand(clientSocket);
		}
		else {
			CommandHandler handler = commandToHandler[command];
			(this->*handler)(clientSocket, tokens);
		}
	}

	void Server::unknownCommand(SocketRAII& clientSocket) {
		string unknown_response = "UNKNOWN COMMAND";
		clientSocket.send(unknown_response);
	}

	void Server::handlePing(SocketRAII& clientSocket, vector<string> args) {
		string ping_response = "PONG";
		int sent = clientSocket.send(ping_response);
	}

	void Server::handleRun(SocketRAII& clientSocket, vector<string> args) {
		string executePath = args[1];
		string params = join(vector<string>(args.begin() + 2, args.end()), ' ');
		std::cout << "Executing: " << executePath << " with params: " << params << std::endl;

		checkError((INT_PTR)(ShellExecuteA(NULL, NULL, executePath.c_str(), params.c_str(), NULL, SW_NORMAL)), "ShellExecuteA");
		
		int sent = clientSocket.send("DONE");
	}

	void Server::handleClient(SocketRAII& clientSocket) {
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
		SocketRAII clientSocket = acceptClient();

		handleClient(clientSocket);
	}

} // namespace server
