#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdexcept>
#include <utility>

using std::exchange;
using std::swap;

using std::runtime_error;

namespace addrinfo_raii {

    /**
     * @brief RAII class for managing addrinfo structures.
     * This class ensures that the addrinfo structure is freed when it goes out of scope.
     */
    class AddrInfoRAII
    {
    public:
        /**
         * @brief Constructor for AddrInfoRAII
         * @param node Node name to pass to getaddrinfo
         * @param service Service name to pass to getaddrinfo 
         * @param hints Hints to pass to getaddrinfo
         * @throws std::runtime_error if getaddrinfo fails
         */
        AddrInfoRAII(const char* node, const char* service, const addrinfo  * hints);
        ~AddrInfoRAII();
        AddrInfoRAII(const AddrInfoRAII& other) = delete;
        AddrInfoRAII(AddrInfoRAII&& other) noexcept;
        AddrInfoRAII& operator=(const AddrInfoRAII& other) = delete;
        AddrInfoRAII& operator=(AddrInfoRAII&& other) noexcept;
        addrinfo* get() const;
    private:
        addrinfo* m_addrInfo;
    };

} // namespace addrinfo_raii



