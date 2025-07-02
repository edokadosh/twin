#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcept>
#include <utility>

using std::exchange;
using std::swap;

using std::runtime_error;

namespace addrinfo_guard {

    /**
     * @brief Guard class for managing addrinfo structures.
     * This class ensures that the addrinfo structure is freed when it goes out of scope.
     */
    class AddrInfoGuard
    {
    public:
        /**
         * @brief Constructor for AddrInfoGuard
         * @param node Node name to pass to getaddrinfo
         * @param service Service name to pass to getaddrinfo 
         * @param hints Hints to pass to getaddrinfo
         * @throws std::runtime_error if getaddrinfo fails
         */
        AddrInfoGuard(const char* node, const char* service, const addrinfo  * hints);
        ~AddrInfoGuard();
        AddrInfoGuard(const AddrInfoGuard& other) = delete;
        AddrInfoGuard(AddrInfoGuard&& other) noexcept;
        AddrInfoGuard& operator=(const AddrInfoGuard& other) = delete;
        AddrInfoGuard& operator=(AddrInfoGuard&& other) noexcept;
        addrinfo* get() const;
    private:
        addrinfo* m_addrInfo;
    };

} // namespace addrinfo_guard



