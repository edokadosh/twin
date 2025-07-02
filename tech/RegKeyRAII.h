#pragma once
#include "pch.h"
#include "exceptions.h"

using exceptions::checkStatus;

namespace regkey_raii {

    /**
     * @brief RAII class for managing Windows registry keys.
     */
    class RegKeyRAII{
    public:
        /**
         * @brief Constructor that opens a registry key
         * @param hKey hKey to pass to RegCreateKeyW
         * @param subKey subKey to pass to RegCreateKeyW
         * @throws RegistryErrorException if the key cannot be opened
         */
        RegKeyRAII(HKEY hKey, LPCWSTR subKey);
        ~RegKeyRAII();

        RegKeyRAII(const RegKeyRAII& other) = delete;
        RegKeyRAII(RegKeyRAII&& other) noexcept = delete;
        RegKeyRAII& operator=(const RegKeyRAII& other) = delete;
        RegKeyRAII& operator=(RegKeyRAII&& other) noexcept = delete;

        void setValue(LPCWSTR valueName, DWORD type, const BYTE* data, DWORD dataSize);
    private:
        HKEY m_hKey;
    };


}


