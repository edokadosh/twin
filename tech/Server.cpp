#include "Server.h"

namespace server {

Server::Server() { return; }

Server::~Server()
{
    WSACleanup();
    closesocket(m_listenSocket);
    for (auto s : m_clientSockets) {
        closesocket(s);
    }
}

void Server::listen()
{
    WSADATA wsaData;
    int iResult;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        throw std::runtime_error("WSAStartup failed");
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
        throw std::runtime_error("getaddrinfo failed");
    }

    // Create a SOCKET for the server to listen for client connections.
    m_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (m_listenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        throw std::runtime_error("socket failed");
    }

    // Setup the TCP listening socket
    iResult = bind(m_listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        throw std::runtime_error("socket failed");
    }

    freeaddrinfo(result);
}

SOCKET Server::acceptClient()
{
    SOCKET clientSocket = accept(m_listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        throw std::runtime_error("accept failed");
    }
    m_clientSockets.push_back(clientSocket);
    return clientSocket;
}

void Server::removeClient(SOCKET clientSocket)
{
    m_clientSockets.erase(
        std::remove(m_clientSockets.begin(), m_clientSockets.end(), clientSocket),
        m_clientSockets.end());
    closesocket(clientSocket);
}

void Server::handleClietn(SOCKET clientSocket)
{
    int iResult;
    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN + 1];
    int recvbuflen = DEFAULT_BUFLEN;

    do {
        iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Echo the buffer back to the sender
            iSendResult = send(clientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                removeClient(clientSocket);

                throw std::runtime_error("send failed");
            }
            printf("Bytes sent: %d\n", iSendResult);
        } else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            removeClient(clientSocket);
            throw std::runtime_error("recv failed");
        }

        if (iResult < 0) {
            removeClient(clientSocket);
            throw std::runtime_error("recv failed");
        } else if (iResult == 0) {
            cout << "Connection closing..." << endl;
            continue;
        }  
        recvbuf[DEFAULT_BUFLEN] = '\0';
        string commandString(recvbuf);

        if (commandToHandler.find(commandString) == commandToHandler.end()) {
            cout << "Got Unknown command" << endl;
        }


    } while (iResult > 0);

    removeClient(clientSocket);
}

void Server::start()
{
    listen();
    SOCKET clientSocket = acceptClient();

    handleClietn(clientSocket);
}

} // namespace server
