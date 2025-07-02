#include "AddrInfoGuard.h"

namespace addrinfo_guard {

    AddrInfoGuard::AddrInfoGuard(const char* node, const char* service, const addrinfo* hints) {
        if (getaddrinfo(node, service, hints, &m_addrInfo) != 0) {
            throw runtime_error("getaddrinfo failed");
        }
    }

    AddrInfoGuard::~AddrInfoGuard() {
        freeaddrinfo(m_addrInfo);
    }

    AddrInfoGuard::AddrInfoGuard(AddrInfoGuard&& other) noexcept
        : m_addrInfo(exchange(other.m_addrInfo, nullptr)) {
        //Left blank intentionally
    }

    AddrInfoGuard& AddrInfoGuard::operator=(AddrInfoGuard&& other) noexcept {
        swap(m_addrInfo, other.m_addrInfo);
        return *this;
    }

    addrinfo* AddrInfoGuard::get() const {
        return m_addrInfo;
    }

} // namespace addrinfo_raii