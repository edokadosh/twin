#include "SocketRAII.h"

namespace socket_raii {

    SocketRAII::SocketRAII() : m_socket(INVALID_SOCKET) {
        // Left blank intentionally
    }

    SocketRAII::SocketRAII(SOCKET sock) : m_socket(sock) {
        if (m_socket == INVALID_SOCKET) {
            throw SocketException("Invalid socket");
        }
    }

    SocketRAII::~SocketRAII() {
        if (m_socket != INVALID_SOCKET) {
            closesocket(m_socket);
        }
    }

    SocketRAII::SocketRAII(SocketRAII&& other) noexcept
        : m_socket(exchange(other.m_socket, INVALID_SOCKET)) {
        // Left blank intentionally
    }

    SocketRAII& SocketRAII::operator=(SocketRAII&& other) noexcept {
        swap(m_socket, other.m_socket);
        return *this;
    }

    bool SocketRAII::operator==(const SocketRAII& other) {
        return m_socket == other.m_socket;
    }

    SOCKET SocketRAII::get() const {
        return m_socket;
    }

    int SocketRAII::send(const char* buf, int len, int flags) const {
        int result = ::send(m_socket, buf, len, flags);
        if (result == SOCKET_ERROR) {
            throw SocketException("send failed");
        }
        return result;
    }

    int SocketRAII::recv(char* buf, int len, int flags) const {
        int result = ::recv(m_socket, buf, len, flags);
        if (result == SOCKET_ERROR) {
            throw SocketException("recv failed");
        }
        return result;
    }

    int SocketRAII::send(const string& str, int flags) {
        return send(str.c_str(), static_cast<int>(str.length()), flags);
    }

    string SocketRAII::recv(int flags) const {
        char buf[DEFAULT_BUFLEN + 1];
        int result = recv(buf, DEFAULT_BUFLEN, flags);
        if (result > 0) {
            buf[result] = '\0'; 
            return string(buf);
        }
        return string(); 
    }

} // namespace socket_raii