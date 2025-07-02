#include "RegKeyRAII.h"

namespace regkey_raii {

    RegKeyRAII::RegKeyRAII(HKEY hKey, LPCWSTR subKey) : m_hKey(NULL) {
        checkStatus(RegCreateKeyW(hKey, subKey, &m_hKey), "RegCreateKeyW");
    }

    RegKeyRAII::~RegKeyRAII() {
        if (m_hKey) {
            checkStatus(RegCloseKey(m_hKey), "RegCloseKey");
        }
    }

    void RegKeyRAII::setValue(LPCWSTR valueName, DWORD type, const BYTE* data, DWORD dataSize) {
        checkStatus(RegSetValueExW(m_hKey, valueName, 0, type, data, dataSize), "RegSetValueExW");
    }

} // namespace regkey_raii