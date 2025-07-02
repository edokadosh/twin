#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcept>
#include <utility>
#include <string>

using std::exchange;
using std::swap;
using std::string;

using std::runtime_error;

namespace socket_raii {

	const size_t DEFAULT_BUFLEN = 512;

    class SocketException : public runtime_error {
    public:
        SocketException(const std::string& msg) : runtime_error(msg) {}
    };

    /**
     * @brief RAII class for managing socket resources.
     */
    class SocketRAII {
    public:
        /**
         * @brief Constructor that initializes the socket.
         * @param sock The socket to manage.
         * @throws SocketException if the socket is invalid.
         */
        SocketRAII();
        SocketRAII(SOCKET sock);
        ~SocketRAII();
        SocketRAII(const SocketRAII& other) = delete;
        SocketRAII(SocketRAII&& other) noexcept;
        SocketRAII& operator=(const SocketRAII& other) = delete;
        SocketRAII& operator=(SocketRAII&& other) noexcept;

        bool operator==(const SocketRAII & other);

        SOCKET get() const;

        int send(const char* buf, int len, int flags) const;
        int recv(char* buf, int len, int flags) const;
        int send(const string& str, int flags = 0);
        string recv(int flags = 0) const;
    private:
        SOCKET m_socket;
    };

} // namespace socket_raii

