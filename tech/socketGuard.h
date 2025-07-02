#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcept>
#include <utility>
#include <string>

using std::exchange;
using std::swap;
using std::string;

namespace socket_guard {

	const size_t DEFAULT_BUFLEN = 512;

    /**
     * @brief Guard class for managing socket resources.
     */
    class SocketGuard {
    public:
        /**
         * @brief Constructor that initializes the socket.
         * @param sock The socket to manage.
         * @throws WinSockErrorException if the socket is invalid.
         */
        SocketGuard();
        SocketGuard(SOCKET sock);
        ~SocketGuard();
        SocketGuard(const SocketGuard& other) = delete;
        SocketGuard(SocketGuard&& other) noexcept;
        SocketGuard& operator=(const SocketGuard& other) = delete;
        SocketGuard& operator=(SocketGuard&& other) noexcept;

        bool operator==(const SocketGuard & other);

        SOCKET get() const;

        int send(const char* buf, int len, int flags) const;
        int recv(char* buf, int len, int flags) const;
        int send(const string& str, int flags = 0);
        string recv(int flags = 0) const;
    private:
        SOCKET m_socket;
    };

} // namespace socket_guard

