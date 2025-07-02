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
using std::runtime_error;

using socket_Guard::SocketGuard;

namespace server {

	const PCSTR DEFAULT_PORT = "12345";
	const string MESSAGE_PING_RESPONSE = "PONG";
	const string MESSAGE_DONE = "DONE";
	const string MESSAGE_READY = "READY";
	const string MESSAGE_FILE = "FILE";

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

	/**
	 * @brief Converts a string to an integer safely
	 * @param str The string to convert
	 * @return The integer value of the string if valid, otherwise returns NOT_A_NUMBER.
	 */
	int safeStoi(string str);

	const int NOT_A_NUMBER = -1;

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

		void handlePing(SocketGuard& clientSocket, vector<string> args);
		void handleRun(SocketGuard& clientSocket, vector<string> args);
		void handleUpload(SocketGuard& clientSocket, vector<string> args);
		void handleDownload(SocketGuard& clientSocket, vector<string> args);

		using CommandHandler = void (Server::*)(SocketGuard&, vector<string> args);
		map<string, CommandHandler> commandToHandler;
	};

}  // namespace server
