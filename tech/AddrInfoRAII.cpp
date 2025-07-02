#include "AddrInfoRAII.h"

namespace addrinfo_raii {

    AddrInfoRAII::AddrInfoRAII(const char* node, const char* service, const addrinfo* hints) {
        if (getaddrinfo(node, service, hints, &m_addrInfo) != 0) {
            throw runtime_error("getaddrinfo failed");
        }
    }

    AddrInfoRAII::~AddrInfoRAII() {
        freeaddrinfo(m_addrInfo);
    }

    AddrInfoRAII::AddrInfoRAII(AddrInfoRAII&& other) noexcept
        : m_addrInfo(exchange(other.m_addrInfo, nullptr)) {
        //Left blank intentionally
    }

    AddrInfoRAII& AddrInfoRAII::operator=(AddrInfoRAII&& other) noexcept {
        swap(m_addrInfo, other.m_addrInfo);
        return *this;
    }

    addrinfo* AddrInfoRAII::get() const {
        return m_addrInfo;
    }

} // namespace addrinfo_raii