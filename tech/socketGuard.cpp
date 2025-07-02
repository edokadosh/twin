#include "SocketGuard.h"
#include <iostream>
#include "exceptions.h"

using std::cerr;
using std::endl;

using exceptions::checkWinSockError;
using exceptions::WinSockErrorException;

namespace socket_Guard {

    SocketGuard::SocketGuard() : m_socket(INVALID_SOCKET) {
        // Left blank intentionally
    }

    SocketGuard::SocketGuard(SOCKET sock) : m_socket(sock) {
        if (m_socket == INVALID_SOCKET) {
            throw WinSockErrorException("Invalid socket");
        }
    }

    SocketGuard::~SocketGuard() {
        if (m_socket != INVALID_SOCKET) {
            try {
                checkWinSockError(closesocket(m_socket), "closesocket");
            }
            catch (const std::runtime_error& e) {
                cerr << "SocketGuard destructor failed: " << e.what() << endl;
            }
        }
    }

    SocketGuard::SocketGuard(SocketGuard&& other) noexcept
        : m_socket(exchange(other.m_socket, INVALID_SOCKET)) {
        // Left blank intentionally
    }

    SocketGuard& SocketGuard::operator=(SocketGuard&& other) noexcept {
        swap(m_socket, other.m_socket);
        return *this;
    }

    bool SocketGuard::operator==(const SocketGuard& other) {
        return m_socket == other.m_socket;
    }

    SOCKET SocketGuard::get() const {
        return m_socket;
    }

    int SocketGuard::send(const char* buf, int len, int flags) const {
        int result = checkWinSockError(::send(m_socket, buf, len, flags), "send");
        return result;
    }

    int SocketGuard::recv(char* buf, int len, int flags) const {
        int result = checkWinSockError(::recv(m_socket, buf, len, flags), "recv");
        return result;
    }

    int SocketGuard::send(const string& str, int flags) {
        return send(str.c_str(), static_cast<int>(str.length()), flags);
    }

    string SocketGuard::recvString(int flags) const {
        char buf[DEFAULT_BUFLEN + 1];
        int result = recv(buf, DEFAULT_BUFLEN, flags);
        if (result > 0) {
            buf[result] = '\0';
            return string(buf);
        }
        return string(); 
    }

    vector<char> SocketGuard::recvBytes(int len, int flags) const {
        vector<char> result;
        vector<char> buffer(DEFAULT_BUFLEN);
        int bytesRead = 0;
        int totalRead = 0;

        do {
            bytesRead = recv(buffer.data(), DEFAULT_BUFLEN, flags);
            if (bytesRead > 0) {
                result.insert(result.end(), buffer.begin(), buffer.begin() + bytesRead);
            }
            totalRead += bytesRead;
        } while ((bytesRead > 0) && (totalRead < len));

        result.shrink_to_fit();
        return result;
    }

    void SocketGuard::sendBytes(vector<char> bytes) {
        int totalSent = 0;
        int bytesToSend = static_cast<int>(bytes.size());

        while (totalSent < bytesToSend) {
            totalSent += send(bytes.data() + totalSent, bytesToSend - totalSent, 0);
        }
    }


} // namespace socket_Guard