#pragma once
#include "pch.h"
#include "exceptions.h"
#include <string>

using std::wstring;

using exceptions::checkStatus;

namespace regkey_guard {

    /**
     * @brief RAII class for managing Windows registry keys.
     */
    class RegKeyGuard{
    public:
        /**
         * @brief Constructor that opens a registry key
         * @param hKey hKey to pass to RegCreateKeyW
         * @param subKey subKey to pass to RegCreateKeyW
         * @throws RegistryErrorException if the key cannot be opened
         */
        RegKeyGuard(HKEY hKey, const wstring& subKey);
        ~RegKeyGuard();

        RegKeyGuard(const RegKeyGuard& other) = delete;
        RegKeyGuard(RegKeyGuard&& other) noexcept = delete;
        RegKeyGuard& operator=(const RegKeyGuard& other) = delete;
        RegKeyGuard& operator=(RegKeyGuard&& other) noexcept = delete;

        void setValue(const wstring& valueName, DWORD type, const BYTE* data, DWORD dataSize);
    private:
        HKEY m_hKey;
    };

} // namespace regkey_guard


