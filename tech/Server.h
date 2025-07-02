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

namespace server {

	const PCSTR DEFAULT_PORT = "12345";

	/**
	 * @brief Splits a string into tokens based on a delimiter
	 * @param str The string to split
	 * @param delimiter The character used to split the string
	 * @return A result vector of tokens
	 */
	vector<string> split(const string& str, char delimiter = ' ');

	/**
	 * @brief Join a vector of strings into a single string with a delimiter
	 * @param tokens The vector of strings to join
	 * @param delimiter The character used to join the strings
	 * @return A single string containing all tokens joined by the delimiter
	 */
	string join(const vector<string>& tokens, char delimiter = ' ');

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

		void handlePing(SocketRAII& clientSocket, vector<string> args);
		void handleRun(SocketRAII& clientSocket, vector<string> args);
		void handleUpload(SocketRAII& clientSocket, vector<string> args);

		using CommandHandler = void (Server::*)(SocketRAII&, vector<string> args);
		map<string, CommandHandler> commandToHandler;
	};

}  // namespace server
