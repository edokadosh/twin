#include "RegKeyGuard.h"
#include <iostream>

using std::cerr;
using std::endl;

namespace regkey_Guard {

    RegKeyGuard::RegKeyGuard(HKEY hKey, const wstring& subKey) : m_hKey(NULL) {
        checkStatus(RegCreateKeyW(hKey, subKey.c_str(), &m_hKey), "RegCreateKeyW");
    }

    RegKeyGuard::~RegKeyGuard() {
        if (m_hKey) {
            try {
                checkStatus(RegCloseKey(m_hKey), "RegCloseKey");
            }
            catch (const std::runtime_error& e) {
                cerr << "RegKeyGuard destructor failed: " << e.what() << endl;
            }
        }
    }

    void RegKeyGuard::setValue(const wstring& valueName, DWORD type, const BYTE* data, DWORD dataSize) {
        checkStatus(RegSetValueExW(m_hKey, valueName.c_str(), 0, type, data, dataSize), "RegSetValueExW");
    }

} // namespace regkey_Guard